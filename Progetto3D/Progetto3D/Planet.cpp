#include "Headers/Planet.h"
#include "Headers/OBJ_Loader.h"

std::shared_ptr<GameObject> Planet::clone()
{
	return std::shared_ptr<Planet>(new Planet(*this));
}

void Planet::update(int time, float deltaTime)
{
	this->transform.objRotation.y += localSpeedRotation * deltaTime;
	this->transform.rotation.y += globalSpeedRotation * deltaTime;
}

objl::Loader Planet::loadModel()
{
	auto loader = GameObject::loadModel();
	if (this->name == "sun") {
		for (auto& m : this->meshes) {
			m.material.fragmentShaderName = "fragmentUnlit";
		}
	}
	return loader;
}

Planet::Planet(std::string const& name, float const localSpeed, float const globalSpeed) : GameObject(), globalSpeedRotation(globalSpeed), localSpeedRotation(localSpeed) {
	this->name = name;
	this->modelPath = "spheres/" + name;
	this->modelName = "planet";
}