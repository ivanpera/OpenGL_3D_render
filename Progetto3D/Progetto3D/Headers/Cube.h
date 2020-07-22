#pragma once
#include "GameObject.h"
class Cube : public GameObject {
public:
	std::shared_ptr<GameObject> clone() override;
	void update(int time, float deltaTime) override;
	objl::Loader loadModel() override;
	Cube();
};