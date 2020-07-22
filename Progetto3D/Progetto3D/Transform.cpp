#include "Headers\Transform.h"

myCommon::vec3 Transform::updateLocalAxis(myCommon::vec3 globalAxis)
{
    float tX = myCommon::degToRad(this->objRotation.x), tY = myCommon::degToRad(this->objRotation.y), tZ = myCommon::degToRad(this->objRotation.z);
    float x = globalAxis.x * (cosf(tZ) * cosf(tY)) + globalAxis.y * (cosf(tZ) * sinf(tY) * sinf(tX) - sinf(tZ) * cosf(tX)) + globalAxis.z * (cosf(tZ) * sinf(tY) * cosf(tX) + sinf(tZ) * sinf(tX));
    float y = globalAxis.x * (sinf(tZ) * cosf(tY)) + globalAxis.y * (sinf(tZ) * sinf(tY) * sinf(tX) + cosf(tZ) * cosf(tX)) + globalAxis.z * (sinf(tZ) * sinf(tY) * cosf(tX) - cosf(tZ) * sinf(tX));
    float z = globalAxis.x * (-sinf(tY))           + globalAxis.y * (cosf(tY) * sinf(tX))                                  + globalAxis.z * (cosf(tY) * cosf(tX));

    return { x, y, z };
}

void Transform::translate(myCommon::vec3 direction, float magnetude)
{
    this->position += direction * magnetude;
}

void Transform::update()
{
    this->right =   updateLocalAxis({ 1,0,0 });
    this->up =      updateLocalAxis({ 0,1,0 });
    this->forward = updateLocalAxis({ 0,0,1 });
}

Transform::Transform()
{
    this->right = { 1,0,0 };
    this->up = { 0,1,0 };
    this->forward = { 0,0,1 };
    this->objRotation = this->rotation = this->position = { 0,0,0 };
    this->scale = { 1,1,1 };
}

bool Transform::operator==(Transform const& other) const
{
    if (this == &other) {
        return true;
    }
    return this->forward == other.forward &&
           this->right == other.right &&
           this->up == other.up &&
           this->objRotation == other.objRotation &&
           this->rotation == other.rotation &&
           this->position == other.position;
}
