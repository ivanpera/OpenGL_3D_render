#pragma once
#include "GameObject.h"
class Planet : public GameObject {
private:
	float localSpeedRotation, globalSpeedRotation;
public:
	std::shared_ptr<GameObject> clone() override;
	void update(int time, float deltaTime) override;
	objl::Loader loadModel() override;
	Planet(std::string const& name, float const localSpeed, float const globalSpeed);
};