#version 400 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec4 aColor; // the color variable has attribute position 1
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec2 uvCoords;

out vec4 ourColor; // output a color to the fragment shader
out vec3 pos;
out vec3 N;
out vec2 uvs;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D alphaTexture;
uniform mat4 Projection;
uniform mat4 Model;
uniform mat4 View;
uniform vec3 cameraPos;
uniform int t;
uniform int numLights;
uniform float dt;
uniform float shininess;

#define MAX_LIGHTS 20

uniform struct Light {
    vec3 position;
    vec3 colorAmbient;
    vec3 colorDiffuse;
    vec3 colorSpecular;
    float constant;
    float linear;
    float quadratic;
} lights[MAX_LIGHTS];

void main()
{
    pos = (Model * vec4(aPos, 1)).xyz;
    N = normalize(Model * vec4(vNormal, 0.0)).xyz;
    gl_Position = Projection*View*Model*vec4(aPos, 1.0);
    //TODO: substitute with material albedo
    ourColor = vec4(1.0);
    uvs = vec2(uvCoords.x, uvCoords.y);
} 