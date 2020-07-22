#include "Headers/World.h"
#include "Headers/Cube.h"
#include <ctime>
#include <memory>
#include "Headers/Planet.h"
using namespace std;


World::World(unsigned int const width, unsigned int const height) {
    using namespace myCommon;
    worldW = width;
    worldH = height;
    float fw = (float)worldW, fh = (float)worldH;
}

void World::onKeyDown(std::string key, int time, float deltaTime) {
    if (gameState == GameState::Over) {
        return;
    }
    std::string space(1, 32);
    if (key._Equal(space)) {
        if (gameState == GameState::Playing) {
            gameState = GameState::Pause;
            std::cout << "PAUSED - Press space to resume\n";
        }
        else if (gameState == GameState::Pause) {
            gameState = GameState::Playing;
            std::cout << "RESUMED - Press space to pause\n";
        }
    }
    for (auto& obj : objects)
    {
        obj->input.setKey(key);
    }
    for (auto& obj : cameras)
    {
        obj->input.setKey(key);
    }
    for (auto& obj : lights)
    {
        obj->input.setKey(key);
    }
}

void World::onKeyRelease(std::string key, int time, float deltaTime)
{
    for (auto& obj : objects)
    {
        obj->input.setKeyUp(key);
    }
    for (auto& obj : cameras)
    {
        obj->input.setKeyUp(key);
    }
    for (auto& obj : lights)
    {
        obj->input.setKeyUp(key);
    }
}

void World::update(int const time, float const deltaTime)
{
    //Also update lights
    for (auto& obj : cameras) {
        obj->physicsUpdate(time, deltaTime);
        obj->update(time, deltaTime);
        obj->input.update();
    }
    for (auto& obj : objects) {
        if (gameState == GameState::Playing) {
            obj->physicsUpdate(time, deltaTime);
            obj->update(time, deltaTime);
        }
        obj->input.update();
    }
    for (auto& obj : lights) {
        if (gameState == GameState::Playing) {
            obj->physicsUpdate(time, deltaTime);
            obj->update(time, deltaTime);
        }
        obj->input.update();
    }
}

void World::loadAssets()
{
    Cube skyBox = Cube();
    skyBox.transform.scale *= {5000, 5000, 5000};
    skyBox.name = "skybox";
    skyBox.modelPath = "cube";
    skyBox.modelName = "cube";
    this->addAsset(skyBox, "Skybox");

    this->addAsset(Planet("sun", 25, 0), "Sun");
    this->addAsset(Planet("earth", 45, 20), "Earth");
    this->addAsset(Planet("moon", 65, 30), "Moon");
}

void World::loadScene()
{
    std::string planetsDir = "spheres/";

    this->addObject(getAsset("Skybox")->clone());
    auto sun = getAsset("Sun")->clone();
    sun->transform.scale *= {40, 40, 40};
    this->addObject(sun);

    auto earth = getAsset("Earth")->clone();
    earth->transform.position.x = -15 * sun->transform.scale.x;
    earth->transform.scale *= {2, 2, 2};
    this->addObject(earth);

    auto moon = getAsset("Moon")->clone();
    moon->transform.position.x = -10 * earth->transform.scale.x;
    moon->transform.scale *= {.3, .3, .3};
    moon->parent = earth;
    this->addObject(moon);

    Camera cam(45);
    cam.transform.position.z = -20 * sun->transform.scale.z;
    this->addCamera(cam);

    Light light;
    light.colorAmbient  = { 1, 1, 1 };
    light.colorDiffuse  = { 1, 1, 1 };
    light.colorSpecular = { 1, 1, 1 };
    light.constantAttenuation  = 0.00002f;
    light.linearAttenuation    = 0.000014f;
    light.quadraticAttenuation = 0.00001f;
    light.transform.position.y = sun->transform.scale.y;
    this->addLight(light);
    light.transform.position.y = -sun->transform.scale.y;
    this->addLight(light);
    light.transform.position.y = 0;
    light.transform.position.z = sun->transform.scale.z;
    this->addLight(light);
    light.transform.position.z = -sun->transform.scale.z;
    this->addLight(light);
    light.transform.position.z = 0;
    light.transform.position.x = sun->transform.scale.x;
    this->addLight(light);
    light.transform.position.x = -sun->transform.scale.x;
    this->addLight(light);
}

std::vector<std::shared_ptr<GameObject>> const& World::getObjects() const
{
    return this->objects;
}

std::vector<std::shared_ptr<Light>> const& World::getLights() const
{
    return this->lights;
}

std::vector<std::shared_ptr<Camera>> const& World::getCameras() const
{
    return this->cameras;
}

std::vector<std::shared_ptr<GameObject>> const World::getAssets() const {
    std::vector<std::shared_ptr<GameObject>> res;
    for (auto it = assets.begin(); it != assets.end(); ++it) {
        res.push_back(it->second);
    }
    return res;
}

std::shared_ptr<GameObject> const& World::getAsset(std::string name) const
{
    auto const& it = this->assets.find(name);
    if (it == assets.end()) {
        throw "No asset with that name found";
    }
    return it->second;
}

std::shared_ptr<Camera>& World::addCamera(Camera const& obj)
{
    cameras.push_back(std::shared_ptr<Camera>(new Camera(obj)));
    return cameras.back();
}

std::shared_ptr<Light>& World::addLight(Light const& obj)
{
    lights.push_back(std::shared_ptr<Light>(new Light(obj)));
    return lights.back();
}

