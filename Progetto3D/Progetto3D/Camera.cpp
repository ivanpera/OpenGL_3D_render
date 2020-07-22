#include "Headers\Camera.h"


/*
    Controls:
    W, S: move forward/backwards
    A, D: strafe left/right
    Q, E: roll left/right
    Up/Down arrow key: pitch up/down
    Left/Right arrow key: rotate left/right
    R,F: move up/down
    TAB: change rotation coordinate system
    SPACE: pause the simulation
    +/-: increase/decrease camera movement speed
    è/à: increase/decraase camera turn speed
    ,/.: increase/decrease camera fov
*/
void Camera::update(int time, float deltaTime)
{
    std::string tab(1, 011);
    if (input.getKeyDown(tab)) {
        this->localRotation = !this->localRotation;
        std::string system = this->localRotation ? "Local" : "Global";
        std::cout << "CHANGED CAMERA ROTATION: " <<  system << " axis\n";
    }
    if (input.getKey("+") || input.getKeyDown("+")) {
        this->movementSpeed += 2 * deltaTime;
        std::cout << "CAMERA MOVEMENT SPEED: " << this->movementSpeed << "\n";
    }
    if (input.getKey("-") || input.getKeyDown("-")) {
        this->movementSpeed -= 2 * deltaTime;
        std::cout << "CAMERA MOVEMENT SPEED: " << this->movementSpeed << "\n";
    }
    if (input.getKey("è") || input.getKeyDown("è")) {
        this->rotationSpeed += 1.0f * deltaTime;
        std::cout << "CAMERA ROTATION SPEED: " << this->rotationSpeed << "\n";
    }
    if (input.getKey("è") || input.getKeyDown("à")) {
        this->rotationSpeed += 1.0f * deltaTime;
        std::cout << "CAMERA ROTATION SPEED: " << this->rotationSpeed << "\n";
    }
    if (input.getKey(",") || input.getKeyDown(",")) {
        this->fovy_degrees += 1.0f * deltaTime;
        std::cout << "CAMERA FOV: " << this->fovy_degrees << "\n";
    }
    if (input.getKey(".") || input.getKeyDown(".")) {
        this->fovy_degrees -= 1.0f * deltaTime;
        std::cout << "CAMERA FOV: " << this->fovy_degrees << "\n";
    }
    if (input.getKeyDown("w") || input.getKey("w")) {
        this->transform.translate(this->transform.forward, this->movementSpeed * deltaTime);
    }
    if (input.getKeyDown("s") || input.getKey("s")) {
        this->transform.translate(this->transform.forward, -this->movementSpeed * deltaTime);
    }
    if (input.getKeyDown("a") || input.getKey("a")) {
        this->transform.translate(this->transform.right, this->movementSpeed * deltaTime);
    }
    if (input.getKeyDown("d") || input.getKey("d")) {
        this->transform.translate(this->transform.right, -this->movementSpeed * deltaTime);
    }
    if (input.getKeyDown("r") || input.getKey("r")) {
        this->transform.translate(this->transform.up, this->movementSpeed * deltaTime);
    }
    if (input.getKeyDown("f") || input.getKey("f")) {
        this->transform.translate(this->transform.up, -this->movementSpeed * deltaTime);
    }
    if (input.getKeyDown("q") || input.getKey("q")) {
        if (this->localRotation) {
            this->transform.objRotation.z -= this->rotationSpeed * deltaTime;
        }
        else {
            this->transform.rotation.z -= this->rotationSpeed * deltaTime;
        }
    }
    if (input.getKeyDown("e") || input.getKey("e")) {
        if (this->localRotation) {
            this->transform.objRotation.z += this->rotationSpeed * deltaTime;
        }
        else {
            this->transform.rotation.z += this->rotationSpeed * deltaTime;
        }
    }
    if (input.getKeyDown("arrowLeft") || input.getKey("arrowLeft")) {
        if (this->localRotation) {
            this->transform.objRotation.y += this->rotationSpeed * deltaTime;
        }
        else {
            this->transform.rotation.y += this->rotationSpeed * deltaTime;
        }
    }
    if (input.getKeyDown("arrowRight") || input.getKey("arrowRight")) {
        if (this->localRotation) {
            this->transform.objRotation.y -= this->rotationSpeed * deltaTime;
        }
        else {
            this->transform.rotation.y -= this->rotationSpeed * deltaTime;
        }
    }
    if (input.getKeyDown("arrowUp") || input.getKey("arrowUp")) {
        if (this->localRotation) {
            this->transform.objRotation.x -= this->rotationSpeed * deltaTime;
        }
        else {
            this->transform.rotation.x -= this->rotationSpeed * deltaTime;
        }
    }
    if (input.getKeyDown("arrowDown") || input.getKey("arrowDown")) {
        if (this->localRotation) {
            this->transform.objRotation.x += this->rotationSpeed * deltaTime;
        }
        else {
            this->transform.rotation.x += this->rotationSpeed * deltaTime;
        }
    }
}

Camera::Camera(float fovy_degrees, float nearLos, float farLos) : fovy_degrees(fovy_degrees), nearLos(nearLos), farLos(farLos)
{
}
