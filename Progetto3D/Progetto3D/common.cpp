#include "Headers/common.h"
#include <string>
#include <cmath>
using namespace myCommon;
float myCommon::degToRad(float const& grad) {
	return grad * PI / 180;
}

std::string const myCommon::vecToString(vec3 vec, std::string name) {
	return name + ": " + std::to_string(vec.x) + " " + std::to_string(vec.y) + " " + std::to_string(vec.z);
}

float const myCommon::signAngleCos(float grad) {
	return std::cosf(degToRad(grad)) >= 0 ? 1.0f : -1.0f;
}

float const myCommon::signAngleSin(float grad) {
	return std::sinf(degToRad(grad)) >= 0 ? 1.0f : -1.0f;
}