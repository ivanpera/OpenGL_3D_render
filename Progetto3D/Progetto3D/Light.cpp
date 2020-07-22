#include "Headers\Light.h"

Light::Light()
{
    this->colorAmbient = { 0,0,0 };
    this->colorDiffuse = { 0,0,0 };
    this->colorAmbient = { 0,0,0 };
    this->constantAttenuation = this->linearAttenuation = this->quadraticAttenuation = 0;
}
