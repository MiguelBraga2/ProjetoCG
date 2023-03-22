#define _USE_MATH_DEFINES

#include "camera.hpp"
#include <math.h>

Camera::Camera(const Point &position, const Point &lookAtPosition, const Point &upVector, float fov, float near,float far) {
    this->position = position;
    this->lookAtPosition = lookAtPosition;
    this->upVector = upVector;
    this->fov = fov;
    this->near = near;
    this->far = far;
    this->cameraRadius = sqrt(pow(this->position.getX()-this->lookAtPosition.getX(), 2) +
                        pow(this->position.getY()-this->lookAtPosition.getY(), 2) +
                        pow(this->position.getZ()-this->lookAtPosition.getZ(), 2));
    this->increment = 0.01;
    this->mode = 0;
    calculateBeta();
    calculateAlpha();
}

void Camera::spherical2Cartesian() {
    this->position.setX(this->cameraRadius * cos(this->beta) * sin(this->alfa));
    this->position.setY(this->cameraRadius * sin(this->beta));
    this->position.setZ(this->cameraRadius * cos(this->beta) * cos(this->alfa));
}

void Camera::incrementAlfa(){
    this->alfa += this->increment;
    if (this->mode == 0){
        this->spherical2Cartesian();
    }
    else if (this->mode == 1){
        this->calculateDirection();
    }

}

void Camera::decrementAlfa(){
    this->alfa -= this->increment;
    if (this->mode == 0){
        this->spherical2Cartesian();
    }
    else if (this->mode == 1){
        this->calculateDirection();
    }

}

void Camera::incrementBeta(){
    this->beta += this->increment;
    if (beta > M_PI/2)
        beta = M_PI/2;
    if (this->mode == 0){
        this->spherical2Cartesian();
    }
    else if (this->mode == 1){
        this->calculateDirection();
    }
}

void Camera::decrementBeta(){
    this->beta -= this->increment;
    if (beta < -M_PI/2)
        beta = -M_PI/2;
    if (this->mode == 0){
        this->spherical2Cartesian();
    }
    else if (this->mode == 1){
        this->calculateDirection();
    }
}

void Camera::incrementRadius(){
    this->cameraRadius += this->increment;
    this->spherical2Cartesian();
}

void Camera::decrementRadius(){
    this->cameraRadius -= this->increment;
    if (cameraRadius < 0.1f)
        cameraRadius = 0.1f;
    this->spherical2Cartesian();
}

void Camera::incrementIncrement() {
    this->increment += 0.01;
}

void Camera::decrementIncrement() {
    this->increment -= 0.01;
    if (this->increment <= 0){
        this->increment = 0.01;
    }
}

void Camera::calculateDirection() {
    this->d.setX(this->cameraRadius * cos(beta) * sin(alfa));
    this->d.setY(this->cameraRadius * sin(beta));
    this->d.setZ(this->cameraRadius * cos(beta) * cos(alfa));
}

void Camera::moveForwards() {
    if (mode == 1){ // Modo FPS
        this->position.setX(this->position.getX() + this->d.getX());
        this->position.setY(this->position.getY() + this->d.getY());
        this->position.setZ(this->position.getZ() + this->d.getZ());
    }
}

void Camera::moveBackwards() {
    if (mode == 1){ // Modo FPS
        this->position.setX(this->position.getX() - this->d.getX());
        this->position.setY(this->position.getY() - this->d.getY());
        this->position.setZ(this->position.getZ() - this->d.getZ());
    }
}

void Camera::changeMode(){
    if (this->mode == 0){
        this->mode = 1;
        this->calculateDirection();
    }
    else if (this->mode = 1){
        this->mode = 0;
    }
}

int Camera::getMode() {
    return mode;
}

void Camera::calculateBeta(){
    // r * sin(beta) = y
    // sin(beta) = y/r
    // beta = arcsin(y/r)

    float acsin = asin(this->position.getY()/this->cameraRadius);
    beta = acsin; // By default between -PI/2 and PI/2 because of arcsin domain
}

void Camera::calculateAlpha(){
    if (position.getX() == 0 && position.getZ() == 0){
        position.setZ(0.01);
    }
    float camPositionY = this->lookAtPosition.getY(); // Projection in the same y-plane as the lookAtPoint
    float zSide = this->position.getZ()-this->lookAtPosition.getZ(); // Length of the side of the triangle parallel to the z axis
    float xSide = this->position.getX()-this->lookAtPosition.getX(); // Length of the side of the triangle parallel to the x axis
    float hip = sqrt(pow(this->position.getX()-this->lookAtPosition.getX(), 2) + pow(this->position.getZ()-this->lookAtPosition.getZ(), 2)); // the hypotenuse

    float accos = acos(zSide/hip); // Angle of Beta (if in the 1st or 2nd quadrants)

    if (xSide < 0){ // Adjust if the angle is in the 3rd or 4th quadrants
        alfa = M_PI - accos + M_PI;
    }
    else {
        alfa = accos;
    }
}

Point Camera::getPosition() {
    return position;
}

Point Camera::getLookAtPosition() {
    return lookAtPosition;
}

Point Camera::getUpVector() {
    return upVector;
}

float Camera::getFov() {
    return fov;
}

float Camera::getNear() {
    return near;
}

float Camera::getFar() {
    return far;
}

Point Camera::getD() {
    return d;
}


