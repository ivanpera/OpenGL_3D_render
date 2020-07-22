#include "Headers/Cube.h"
#include "Headers/OBJ_Loader.h"

std::shared_ptr<GameObject> Cube::clone()
{
	return std::shared_ptr<Cube>(new Cube(*this));
}

void Cube::update(int time, float deltaTime)
{
}

objl::Loader Cube::loadModel()
{
	auto loader = GameObject::loadModel();
	if (this->name == "skybox") {
		for (auto& m : this->meshes) {
			m.material.fragmentShaderName = "fragmentUnlit";
		}
	}
	return loader;
}

Cube::Cube() : GameObject() {
}