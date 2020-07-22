#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Transform.h"
#include "Material.h"
#include "Mesh.h"
#include "common.h"
#include "OBJ_Loader.h"
#include "Input.h"
#include <optional>

class GameObject {
public:
    virtual objl::Loader loadModel();
    virtual void physicsUpdate(int time, float deltaTime);
    virtual void update(int time, float deltaTime);
    void addChild(std::shared_ptr<GameObject>const& object);
    void removeChild(std::shared_ptr<GameObject>const& object);
    void setParent(std::shared_ptr<GameObject>const& object);
    Transform transform;
    Input input;
    std::vector<Mesh> meshes;
    std::string modelName, modelPath, name;
    std::optional<std::shared_ptr<GameObject>> parent;
    std::vector<std::shared_ptr<GameObject>> children;
    virtual std::shared_ptr<GameObject> clone();
    bool operator == (GameObject const& other) const;
    //~GameObject(); //-> tell the parent to remove the object form the children collection
protected:
    GameObject(GameObject const& other);
    GameObject();
};