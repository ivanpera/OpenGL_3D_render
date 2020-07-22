#pragma once
#include "GameObject.h"
class Camera : public GameObject {
public:
	float fovy_degrees, nearLos, farLos, movementSpeed = 30, rotationSpeed = 65;
	bool localRotation = true;
	void update(int time, float deltaTime) override;
	Camera(float fovy_degrees = 45,float nearLos = .01f, float farLos = 10000.0f);
};