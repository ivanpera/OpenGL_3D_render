#include "Headers/GameObject.h"
#include <iostream>

//maybe get the file path as parameter
objl::Loader GameObject::loadModel()
{
    // Initialize Loader
    objl::Loader Loader;

    // Load .obj File
    bool loadout = Loader.LoadFile("Models/" + this->modelPath + "/" + this->modelName + ".obj");
    if (!loadout) {
        throw "Couldn't load model at location: Models/" + this->modelPath + "/" + this->modelName + ".obj";
    }
    this->meshes = Loader.LoadedMeshes;
    return Loader;
}

void GameObject::physicsUpdate(int time, float deltaTime)
{
    this->transform.update();
}

void GameObject::update(int time, float deltaTime)
{
}

void GameObject::addChild(std::shared_ptr<GameObject>const& object)
{
    if (*object == *this) {
        throw "You cannot assign yoursef as your children";
    }
    for (auto& c : children) {
        if (*object == *c) {
            throw "You cannot add the same children twice";
        }
    }
    object->parent = { std::shared_ptr<GameObject>(this) };
    children.push_back(object);
}

void GameObject::removeChild(std::shared_ptr<GameObject>const& object)
{
    auto it = std::find_if(children.begin(), children.end(), [&object](std::shared_ptr<GameObject> const& e) {return e == object; });
    if (it != children.end()) {
        (*it)->parent = {};
        children.erase(it);
    }
}

void GameObject::setParent(std::shared_ptr<GameObject>const& object)
{
    if (*object == *this) {
        throw "You cannot assign yoursef as your parent";
    }
    for (auto& c : children) {
        if (*object == *c) {
            throw "You cannot assign one of your children as your parent";
        }
    }
    object->addChild(std::shared_ptr<GameObject>(this));
}

GameObject::GameObject() : transform(), meshes(), modelName(""), modelPath(""), name("Object")
{
}

std::shared_ptr<GameObject> GameObject::clone()
{
    throw "Invalid call to deleted GameObject clone";
}

bool GameObject::operator==(GameObject const& other) const
{
    if (&other == this) {
        return true;
    }
    return this->name._Equal(other.name) &&
           this->transform == other.transform &&
           this->modelName._Equal(other.modelName) &&
           this->modelPath._Equal(other.modelPath) &&
           this->parent == other.parent &&
           this->children == other.children;
}

//GameObject::~GameObject()
//{
//    /*if (this->parent) {
//        (*this->parent)->removeChild(this);
//    }*/
//}

GameObject::GameObject(GameObject const& other)
{
    this->meshes = other.meshes;
    this->transform = other.transform;
    this->modelName = other.modelName;
    this->modelPath = other.modelPath;
    this->name = other.name;
}
