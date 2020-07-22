#pragma once
#include "GameObject.h"
class Light : public GameObject {
public:
	myCommon::vec3 colorAmbient, colorDiffuse, colorSpecular;
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
	Light();
};