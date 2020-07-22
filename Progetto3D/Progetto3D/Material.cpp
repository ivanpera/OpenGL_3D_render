#include "Headers/Material.h"

Material::Material() {
	this->ambient = this->diffuse = this->specular = { 0,0,0 };
	this->shininess = 0;
}