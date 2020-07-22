#pragma once
#include "common.h"
class Transform {
private:
	myCommon::vec3 updateLocalAxis(myCommon::vec3 globalAxis);
public:
	void translate(myCommon::vec3 direction, float magnetude);
	void update();
	myCommon::vec3 position;
	myCommon::vec3 objRotation;
	myCommon::vec3 rotation;
	myCommon::vec3 scale;
	myCommon::vec3 up;
	myCommon::vec3 right;
	myCommon::vec3 forward;
	Transform();
	bool operator == (Transform const& other) const;
};