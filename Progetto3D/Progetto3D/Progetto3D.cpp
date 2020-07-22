#include <iostream>
#include "Headers/ShaderMaker.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <vector>
#include "Headers/ShaderKey.h"
#include "Headers/World.h"
#include "Headers/common.h"
#include "Headers/OBJ_Loader.h"
#include "Headers/stb_image.h"

glm::mat4 Projection;
static constexpr int MAX_LIGHTS = 20;

typedef struct Program {
    unsigned int id;		//points to the shader
    unsigned int modelMatr; //points to the variable in the shader
    unsigned int projMatr;	//points to the variable in the shader
    unsigned int viewMatr;
    unsigned int time;
    unsigned int deltaTime;
    unsigned int lightColorA[MAX_LIGHTS];
    unsigned int lightColorD[MAX_LIGHTS];
    unsigned int lightColorS[MAX_LIGHTS];
    unsigned int lightPosition[MAX_LIGHTS];
    unsigned int attenuationConstant[MAX_LIGHTS];
    unsigned int attenuationLinear[MAX_LIGHTS];
    unsigned int attenuationQuadratic[MAX_LIGHTS];
    unsigned int materialAmbient;
    unsigned int materialDiffuse;
    unsigned int materialSpecular;
    unsigned int materialShininess;
    unsigned int materialAlpha;
    unsigned int cameraPos;
    unsigned int numLights;
} Program;

static constexpr int GLUT_MAIN_VERSION = 4, GLUT_SUB_VERSION = 0;
static constexpr int DEFAULT_WINDOW_WIDTH = 768, DEFAULT_WINDOW_HEIGHT = 756;
static constexpr int DEFAULT_WINDOW_X = 100, DEFAULT_WINDOW_Y = 100;
static constexpr int UPDATE_PER_SECOND = 1000 / 60, PHYSICS_UPDATE_PER_SECOND = 1000 / 30;
static void const setupGlut(int& argc, char** argv, char const* title);
static std::map<ShaderKey, Program> programs;
static std::map<std::string, unsigned int> meshes;
static std::map<std::string, unsigned int> textures;
static Program findOrBuildShader(ShaderKey const& key);
static void loadVAO(GameObject& object);
static void onDisplay(void);
static void loadTextures(GameObject const& object);
static void buildShader(ShaderKey const& key);
static int parseDrawMode(myCommon::DrawMode const& drawMode);
static bool texturesLoaded(GameObject const& object);
static int timestamp = 0;
static float deltaTime = 0;
static int windowW, windowH, windowX, windowY;
static Program program = {};
static std::unique_ptr<World> world;

static void initVAO(void);


void checkError(std::string op) {
    GLenum ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        std::cout << ErrorCheckValue;
        fprintf(
            stderr,
            "ERROR: %s: %s \n",
            op.c_str(),
            gluErrorString(ErrorCheckValue)
        );

        exit(-1);
    }
}

int main(int argc, char** argv)
{
    using namespace glm;
    char const* title = "Titolo";

    setupGlut(argc, argv, title);
    world.reset(new World(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)));
    world->loadAssets();
    initVAO();
    for (auto const& obj : world->getAssets()) {
        loadTextures(*obj);
    }
    world->loadScene();
    std::cout << "Start\n";
    glutMainLoop();
}

glm::vec3 myCommontToGlm(myCommon::vec3 const& vec) {
    return glm::vec3(vec.x, vec.y, vec.z);
}

void onDisplay(void) {
    using namespace glm;
    glClearColor(0.0f, 0.0f, 0.0f, .0f);

    ShaderKey shaderKey = { "", "" };
    std::string vaoKey = "";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto const& camera : world->getCameras()) {
        Projection = glm::perspective(camera->fovy_degrees, (float)windowW / float(windowH), camera->nearLos, camera->farLos);

        ////mat4 buildViewMatrix(camera)

        vec3 camPos = myCommontToGlm(camera->transform.position);
        vec3 camFront = myCommontToGlm(camera->transform.forward);
        vec3 camUp = myCommontToGlm(camera->transform.up);
        glm::mat4 View = lookAt(camPos, camPos + camFront, camUp);
        //For the future: here check if parent if present, in case rotate along parent axis
        View = rotate(View, radians(camera->transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        View = rotate(View, radians(camera->transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        View = rotate(View, radians(camera->transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        ////

        auto const& lights = world->getLights();
        for (auto const& obj : (*world).getObjects()) {
            std::string objVAOName = obj->modelName;
            //bind VAO
            if (vaoKey != objVAOName) {
                glBindVertexArray(0);
                vaoKey = objVAOName;
                if (meshes.count(vaoKey) == 0) {
                    loadVAO(*obj);
                }
                glBindVertexArray(meshes.at(vaoKey));
            }

            if (!texturesLoaded(*obj)) {
                loadTextures(*obj);
            }

            for (auto const& mesh : obj->meshes) {
                glm::vec4 blendingFactors = glm::vec4(0, 0, 0, 0);
                ShaderKey const objKey = { mesh.material.vertexShaderName, mesh.material.fragmentShaderName };
                if (shaderKey != objKey) {
                    shaderKey = objKey;
                    program = findOrBuildShader(shaderKey);
                    glUseProgram(program.id);
                }
                
                std::vector<std::string> textureSources = { mesh.material.map_Ka, mesh.material.map_Kd, mesh.material.map_Ks, mesh.material.map_d };
                int tIdx = 0;
                for (auto const& src : textureSources) {
                    if (textures.count(src) != 0) {
                        glActiveTexture(GL_TEXTURE0 + tIdx);
                        glBindTexture(GL_TEXTURE_2D, textures.at(src));
                        blendingFactors[tIdx] = 1;
                    }
                    tIdx++;
                }

                glUniform1i(glGetUniformLocation(program.id, "ambientTexture"), 0);
                glUniform1i(glGetUniformLocation(program.id, "diffuseTexture"), 1);
                glUniform1i(glGetUniformLocation(program.id, "specularTexture"), 2);
                glUniform1i(glGetUniformLocation(program.id, "alphaTexture"), 3);
                glUniform3f(glGetUniformLocation(program.id, "blendingFactors"), blendingFactors.x, blendingFactors.y, blendingFactors.z);

                glUniform1i(program.time, timestamp);
                glUniform1f(program.deltaTime, deltaTime);
                glUniform3f(program.materialAmbient, mesh.material.Ka.x, mesh.material.Ka.y, mesh.material.Ka.z);
                glUniform3f(program.materialDiffuse, mesh.material.Kd.x, mesh.material.Kd.y, mesh.material.Kd.z);
                glUniform3f(program.materialSpecular, mesh.material.Ks.x, mesh.material.Ks.y, mesh.material.Ks.z);
                glUniform1f(program.materialShininess, mesh.material.Ns);
                glUniform1f(program.materialAlpha, mesh.material.d);
                size_t numLights = lights.size() > MAX_LIGHTS ? MAX_LIGHTS : lights.size();
                glUniform1i(program.numLights, numLights);

                auto it = lights.begin();
                int i = 0;
                for (; it != lights.end() && i < numLights; ++it, ++i) {
                    auto const& position = (*it)->transform.position;
                    glUniform3f(program.lightPosition[i], position.x, position.y, position.z);
                    glUniform3f(program.lightColorA[i], (*it)->colorAmbient.x, (*it)->colorAmbient.y, (*it)->colorAmbient.z);
                    glUniform3f(program.lightColorD[i], (*it)->colorDiffuse.x, (*it)->colorDiffuse.y, (*it)->colorDiffuse.z);
                    glUniform3f(program.lightColorS[i], (*it)->colorSpecular.x, (*it)->colorSpecular.y, (*it)->colorSpecular.z);
                    glUniform1f(program.attenuationConstant[i], (*it)->constantAttenuation);
                    glUniform1f(program.attenuationLinear[i], (*it)->linearAttenuation);
                    glUniform1f(program.attenuationQuadratic[i], (*it)->quadraticAttenuation);
                }
                glUniform3f(program.cameraPos, camera->transform.position.x, camera->transform.position.y, camera->transform.position.z);

                //Send Projection to shader
                glUniformMatrix4fv(programs.at(shaderKey).projMatr, 1, GL_FALSE, value_ptr(Projection));
                //Send View to shader
                glUniformMatrix4fv(programs.at(shaderKey).viewMatr, 1, GL_FALSE, value_ptr(View));

                mat4 Model = mat4(1.0);
                //If the object has a parent, rotate the object by the parent world rotation
                if (obj->parent) {
                    glm::vec3 xAxis, yAxis, zAxis, rotation;
                    xAxis = myCommontToGlm((*obj->parent)->transform.right);
                    yAxis = myCommontToGlm((*obj->parent)->transform.up);
                    zAxis = myCommontToGlm((*obj->parent)->transform.forward);
                    rotation = myCommontToGlm((*obj->parent)->transform.rotation);
                    Model = rotate(Model, radians(rotation.x), xAxis);
                    Model = rotate(Model, radians(rotation.y), yAxis);
                    Model = rotate(Model, radians(rotation.z), zAxis);
                    Model = translate(Model, myCommontToGlm((*(obj->parent))->transform.position));
                }

                //Rotate around world axis
                Model = rotate(Model, radians(obj->transform.rotation.x), { 1,0,0 });
                Model = rotate(Model, radians(obj->transform.rotation.y), { 0,1,0 });
                Model = rotate(Model, radians(obj->transform.rotation.z), { 0,0,1 });

                //If the object has a parent, rotate the object by the parent object rotation
                if (obj->parent) {
                    glm::vec3 xAxis, yAxis, zAxis, rotation;
                    xAxis = myCommontToGlm((*obj->parent)->transform.right);
                    yAxis = myCommontToGlm((*obj->parent)->transform.up);
                    zAxis = myCommontToGlm((*obj->parent)->transform.forward);
                    rotation = myCommontToGlm((*obj->parent)->transform.objRotation);
                    Model = rotate(Model, radians(rotation.x), xAxis);
                    Model = rotate(Model, radians(rotation.y), yAxis);
                    Model = rotate(Model, radians(rotation.z), zAxis);
                }

                //Translate to object position
                Model = translate(Model, myCommontToGlm(obj->transform.position));

                //Rotate relative to object axis
                Model = rotate(Model, radians(obj->transform.objRotation.x), glm::vec3(myCommontToGlm(obj->transform.right)));
                Model = rotate(Model, radians(obj->transform.objRotation.y), glm::vec3(myCommontToGlm(obj->transform.up)));
                Model = rotate(Model, radians(obj->transform.objRotation.z), glm::vec3(myCommontToGlm(obj->transform.forward)));
                Model = scale(Model, myCommontToGlm(obj->transform.scale));
                //Send Model to shader
                glUniformMatrix4fv(programs.at(shaderKey).modelMatr, 1, GL_FALSE, value_ptr(Model));

                glDrawElements(GL_TRIANGLES, mesh.indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, (void *)0);
                checkError("draw");
            }
        }
    
    }

    glBindVertexArray(0);
    glutSwapBuffers();
}

void loadTextures(GameObject const& obj) {
    for (auto const& mesh : obj.meshes) {
        std::vector<std::string> textureSources = { mesh.material.map_Ka, mesh.material.map_Kd, mesh.material.map_Ks, mesh.material.map_d };
        for (auto const& src : textureSources) {
            if (!src._Equal("") && textures.count(src) == 0) {
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                // TODO: might want to find a way to customize textures behaviour when going out of uv range
                // set the texture wrapping/filtering options (on the currently bound texture object)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // load and generate the texture
                int width, height, nrChannels;
                stbi_set_flip_vertically_on_load(true);
                std::string path = "./Models/" + obj.modelPath + "/" + src;
                unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
                if (data)
                {
                    switch (nrChannels)
                    {
                    case 3:
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                        break;
                    case 4:
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                        break;
                    default:
                        throw "Unsupported number of channels";
                    }
                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                else
                {
                    std::cerr << "Failed to load texture" << std::endl;
                    throw "Incorrect file name";
                }
                textures.insert({ src, texture });
                stbi_image_free(data);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }
    }
}

void loadVAO(GameObject& obj) {
    std::string meshKey = obj.modelName;
    //if (meshes.count(meshKey) == 0) {
        objl::Loader const loader = obj.loadModel();
        meshes.insert({ meshKey, 0 });
        unsigned int& VAO = meshes.at(meshKey);
        unsigned int VBO, EBO;
        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, loader.LoadedVertices.size() * sizeof(myCommon::Point), loader.LoadedVertices.data(), GL_STATIC_DRAW);
        //Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //Normal
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        //uv
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, loader.LoadedIndices.size() * sizeof(unsigned int), loader.LoadedIndices.data(), GL_STATIC_DRAW);
        
        glBindVertexArray(0);

        checkError("Mesh loading");
    /*}
    else {
        std::cerr << "WARNING: asset " + obj.name + " with global mesh name " + obj.modelName + " already loaded\n";
    }*/
}

void initVAO(void) {
    ShaderKey last = { "", "" };
    for (auto const& obj : (*world).getAssets()) {

        loadVAO(*obj);
        for (auto const& mesh : obj->meshes) {
            ShaderKey const current = { mesh.material.vertexShaderName, mesh.material.fragmentShaderName };
            if (current != last) {
                findOrBuildShader(current);
                last = current;
            }
            checkError("Shader building");
        }
    }
    std::cout << "initVAO ok\n";
    std::cout << "MeshCount: " << meshes.size() << "\n";
}

void handleKeyboardSpecial(int k, int x, int y, bool keyPressed) {
    std::string key = "nullKey";
    switch (k)
    {
    case GLUT_KEY_UP:
        key = "arrowUp";
        break;
    case GLUT_KEY_RIGHT:
        key = "arrowRight";
        break;
    case GLUT_KEY_DOWN:
        key = "arrowDown";
        break;
    case GLUT_KEY_LEFT:
        key = "arrowLeft";
        break;
    default:
        break;
    }
    if (keyPressed) {
        (*world).onKeyDown(key, timestamp, deltaTime);
    }
    else {
        (*world).onKeyRelease(key, timestamp, deltaTime);
    }
}

void handleKeyboard(unsigned char k, int x, int y, bool keyPressed) {
    std::string key = std::string(1, k);
    //Esc key
    if (k == 033) {
        glutLeaveMainLoop();
    }
    else if (keyPressed) {
        (*world).onKeyDown(key, timestamp, deltaTime);
    }
    else {
        (*world).onKeyRelease(key, timestamp, deltaTime);
    }
    
}

void onKeyDown(unsigned char k, int x, int y) {
    handleKeyboard(k, x, y, true);
}

void onKeyRelease(unsigned char k, int x, int y) {
    handleKeyboard(k, x, y, false);
}

void onSpecialDown(int k, int x, int y) {
    handleKeyboardSpecial(k, x, y, true);
}

void onSpecialRelease(int k, int x, int y) {
    handleKeyboardSpecial(k, x, y, false);
}

void onResize(int w, int h) {
    glViewport(0, 0, w, h);
    windowW = w;
    windowH = h;
}

void physicsUpdate(int v) {
    int oldTime = timestamp;
    timestamp = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = glm::clamp((float)((timestamp - oldTime)) / 1000, 0.0f, (1.0f * PHYSICS_UPDATE_PER_SECOND) / 1000);
    (*world).update(timestamp, deltaTime);

    glutPostRedisplay();
    glutTimerFunc(PHYSICS_UPDATE_PER_SECOND, physicsUpdate, 0);
}

void const setupGlut(int& argc, char** argv, char const* title) {
    using namespace glm;
    glutInit(&argc, argv);
    glutInitContextVersion(GLUT_MAIN_VERSION, GLUT_SUB_VERSION);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    int const screenW = glutGet(GLUT_SCREEN_WIDTH), screenH = glutGet(GLUT_SCREEN_HEIGHT);
    int const percentageW = 80, percentageH = 80;

    if (screenW <= 0 && screenH <= 0) {
        windowW = DEFAULT_WINDOW_WIDTH;
        windowH = DEFAULT_WINDOW_HEIGHT;
        windowX = DEFAULT_WINDOW_X;
        windowY = DEFAULT_WINDOW_Y;
    }
    else {
        windowW = screenW * percentageW / 100;
        windowH = screenH * percentageH / 100;
        windowX = screenW / 2 - windowW / 2;
        windowY = screenH / 2 - windowH / 2;
    }

    glutInitWindowSize(windowW, windowH);
    glutInitWindowPosition(windowX, windowY);
    glutCreateWindow(title);
    glutDisplayFunc(onDisplay);
    glutReshapeFunc(onResize);
    glutKeyboardFunc(onKeyDown);
    glutKeyboardUpFunc(onKeyRelease);
    glutSpecialFunc(onSpecialDown);
    glutSpecialUpFunc(onSpecialRelease);
    glutTimerFunc(PHYSICS_UPDATE_PER_SECOND, physicsUpdate, 0);
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Might experiment?
    checkError("SetupGlut");
    glClearColor(0.0f, 0.4f, 0.5f, .1f);
    std::cout << "SetupGlut ok\n";
}


Program findOrBuildShader(ShaderKey const& key) {
    if (programs.count(key) <= 0) {
        buildShader(key);
    }
    return programs.at(key);
}

int parseDrawMode(myCommon::DrawMode const& drawMode) {
    using namespace myCommon;
    switch (drawMode)
    {
    case DrawMode::lines:
        return GL_LINES;
    case DrawMode::lines_strip:
        return GL_LINE_STRIP;
    case DrawMode::lines_loop:
        return GL_LINE_LOOP;
    case DrawMode::points:
        return GL_POINTS;
    case DrawMode::triangles:
        return GL_TRIANGLES;
    case DrawMode::triangles_fan:
        return GL_TRIANGLE_FAN;
    case DrawMode::triangles_strip:
        return GL_TRIANGLE_STRIP;
    default:
        throw "Unrecognized draw mode";
    }
}

void buildShader(ShaderKey const& key) {
    Program program;
    program.id = ShaderMaker::createProgram(key.vertShader.c_str(), key.fragShader.c_str());
    program.modelMatr = glGetUniformLocation(program.id, "Model");
    program.viewMatr = glGetUniformLocation(program.id, "View");
    program.projMatr = glGetUniformLocation(program.id, "Projection");
    program.time = glGetUniformLocation(program.id, "t");
    program.deltaTime = glGetUniformLocation(program.id, "dt");
    for (int i = 0; i < MAX_LIGHTS; i++) {
        program.lightColorA[i] = glGetUniformLocation(program.id, ("lights[" + std::to_string(i) + "].colorAmbient").c_str());
        program.lightColorD[i] = glGetUniformLocation(program.id, ("lights[" + std::to_string(i) + "].colorDiffuse").c_str());
        program.lightColorS[i] = glGetUniformLocation(program.id, ("lights[" + std::to_string(i) + "].colorSpecular").c_str());
        program.lightPosition[i] = glGetUniformLocation(program.id, ("lights[" + std::to_string(i) + "].position").c_str());
        program.attenuationConstant[i] = glGetUniformLocation(program.id, ("lights[" + std::to_string(i) + "].constant").c_str());
        program.attenuationLinear[i] = glGetUniformLocation(program.id, ("lights[" + std::to_string(i) + "].linear").c_str());
        program.attenuationQuadratic[i] = glGetUniformLocation(program.id, ("lights[" + std::to_string(i) + "].quadratic").c_str());
    }
    program.materialAmbient = glGetUniformLocation(program.id, "materialAmbient");
    program.materialDiffuse = glGetUniformLocation(program.id, "materialDiffuse");
    program.materialSpecular = glGetUniformLocation(program.id, "materialSpecular");
    program.materialShininess = glGetUniformLocation(program.id, "materialShininess");
    program.materialAlpha = glGetUniformLocation(program.id, "materialAlpha");
    program.cameraPos = glGetUniformLocation(program.id, "cameraPos");
    program.numLights = glGetUniformLocation(program.id, "numLights");
    programs.insert({ key, program });
}

bool texturesLoaded(GameObject const& object)
{
    for (auto const& mesh : object.meshes) {
        std::vector<std::string> textureSources = { mesh.material.map_Ka, mesh.material.map_Kd, mesh.material.map_Ks, mesh.material.map_d };
        for (auto const& src : textureSources) {
            if (textures.count(src) == 0) {
                return false;
            }
        }
    }
    return true;
}