#define _USE_MATH_DEFINES

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "camera.hpp"
#include <math.h>
#include "stdio.h"

Camera::Camera(const Point &position, const Point &lookAtPosition, const Point &upVector, float fov, float near,float far) {
    this->position = position;
    this->lookAtPosition = lookAtPosition;
    this->upVector = upVector;
    this->fov = fov;
    this->near = near;
    this->far = far;
    this->mode = 0;
    this->d = Point(0,0,0);
    this->calculateSpherical(); //  Enter spherical mode
}

/**
 * Calculate alfa, beta and radius, necessary for the explorer camera mode
 */
void Camera::calculateSpherical(){
    this->cameraRadius = sqrt(pow(this->position.getX()-this->lookAtPosition.getX(), 2) +
                              pow(this->position.getY()-this->lookAtPosition.getY(), 2) +
                              pow(this->position.getZ()-this->lookAtPosition.getZ(), 2));
    this->increment = 0.01;
    calculateBeta();
    calculateAlfa();
}

/**
 * Given the cameraRadius, alfa and beta, calculate the camera position
 */
void Camera::spherical2Cartesian() {
    this->position.setX(this->cameraRadius * cos(this->beta) * sin(this->alfa));
    this->position.setY(this->cameraRadius * sin(this->beta));
    this->position.setZ(this->cameraRadius * cos(this->beta) * cos(this->alfa));
}

/**
 * Increase the horizontal angle
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
void Camera::incrementAlfa(){
    this->alfa += this->increment;
    if (this->mode == 0){
        this->spherical2Cartesian();
    }
    else if (this->mode == 1){
        this->calculateDirection();
    }
}

/**
 * Decrease the horizontal angle
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
void Camera::decrementAlfa(){
    this->alfa -= this->increment;
    if (this->mode == 0){
        this->spherical2Cartesian();
    }
    else if (this->mode == 1){
        this->calculateDirection();
    }
}

/**
 * Increase the vertical angle
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
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

/**
 * Decrease the vertical angle
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
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

/**
 * Increase the explorer camera radius
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
void Camera::incrementRadius(){
    this->cameraRadius += this->increment;
    this->spherical2Cartesian();
}

/**
 * Decrease the explorer camera radius
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
void Camera::decrementRadius(){
    this->cameraRadius -= this->increment;
    if (cameraRadius < 0.1f)
        cameraRadius = 0.1f;
    this->spherical2Cartesian();
}

/**
 * Increase increment of the explorer camera radius
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
void Camera::incrementIncrement() {
    this->increment += 0.01;
}

/**
 * Decrease increment of the explorer camera radius
 * In explorer mode, update the cartesian coordinates
 * In FPS mode, recalculate direction vector
 */
void Camera::decrementIncrement() {
    this->increment -= 0.01;
    if (this->increment <= 0){
        this->increment = 0.01;
    }
}

/**
 * Calculates the vector of the direction the camera is looking
 * given alfa and beta
 */
void Camera::calculateDirection() {
    this->d.setX(cos(beta) * sin(alfa));
    this->d.setY(sin(beta));
    this->d.setZ(cos(beta) * cos(alfa));
}

/**
 * In FPS mode, goes to the direction it is looking
 */
void Camera::moveForwards() {
    if (mode == 1){ // Modo FPS
        this->position.setX(this->position.getX() + this->d.getX());
        this->position.setY(this->position.getY() + this->d.getY());
        this->position.setZ(this->position.getZ() + this->d.getZ());
    }
}

/**
 * In FPS, goes to the opposite direction it is looking
 */
void Camera::moveBackwards() {
    if (mode == 1){ // Modo FPS
        this->position.setX(this->position.getX() - this->d.getX());
        this->position.setY(this->position.getY() - this->d.getY());
        this->position.setZ(this->position.getZ() - this->d.getZ());
    }
}

void Camera::moveLeft() {
    if (mode == 1){ // Modo FPS
        Point perp = Point::crossProduct(upVector, d);
        perp.normalize();
        this->position.setX(this->position.getX() + perp.getX());
        this->position.setY(this->position.getY() + perp.getY());
        this->position.setZ(this->position.getZ() + perp.getZ());
    }
}

void Camera::moveRight() {
    if (mode == 1){ // Modo FPS
        Point perp = Point::crossProduct(upVector, d);
        perp.normalize();
        this->position.setX(this->position.getX() - perp.getX());
        this->position.setY(this->position.getY() - perp.getY());
        this->position.setZ(this->position.getZ() - perp.getZ());
    }
}

void Camera::moveUp() {
    if (mode == 1){ // Modo FPS
        this->position.setX(this->position.getX() + upVector.getX());
        this->position.setY(this->position.getY() + upVector.getY());
        this->position.setZ(this->position.getZ() + upVector.getZ());
    }
}

void Camera::moveDown() {
    if (mode == 1){ // Modo FPS
        this->position.setX(this->position.getX() - upVector.getX());
        this->position.setY(this->position.getY() - upVector.getY());
        this->position.setZ(this->position.getZ() - upVector.getZ());
    }
}

/**
 * Changes the camera mode
 */
void Camera::changeMode(){
    if (this->mode == 0){
        this->mode = 1;

        this->d = Point((this->lookAtPosition.getX() - this->position.getX()),
                        (this->lookAtPosition.getY() - this->position.getY()),
                        (this->lookAtPosition.getZ() - this->position.getZ()));
        this->d.normalize();
        this->calculateAlfa();
        this->calculateBeta();
        this->calculateDirection();
    }
    else if (this->mode == 1){
        this->mode = 2;
        this->calculateSpherical();
    }
    else if (this->mode == 2){
        this->mode = 0;
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

/**
 * Updates camera angles based on motion of the mouse
 * @param tracking type of motion
 * @param horizDisp displacement of the mouse horizontally
 * @param vertDisp displacement of the mouse vertically
 */
void Camera::updateMouseAngles(int tracking, int horizDisp, int vertDisp){
    if (mode == 2) {
        if (tracking == 1) {
            alfa += horizDisp;
            beta += vertDisp;
        }
        else if (tracking == 2) {
            cameraRadius -= vertDisp;
            if(cameraRadius < 3){
                cameraRadius = 3;
            }
        }
    }
}

void Camera::processMouseMotion(int tracking, int horizDisp, int vertDisp) {
    if (mode == 2){
        int alphaAux, betaAux;
        int rAux;

        if (!tracking)
            return;

        if (tracking == 1) {
            alphaAux = alfa + horizDisp;
            betaAux = beta + vertDisp;

            if (betaAux > 85.0)
                betaAux = 85.0;
            else if (betaAux < -85.0)
                betaAux = -85.0;

            rAux = cameraRadius;
        }
        else if (tracking == 2) {

            alphaAux = alfa;
            betaAux = beta;
            rAux = cameraRadius - vertDisp;
            if (rAux < 3)
                rAux = 3;
        }
        this->position = Point(rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0),
                               rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0),
                               rAux * 							      sin(betaAux * 3.14 / 180.0));
    }
}

/**
 * Places the camera in the world, using the glut gluLookAt function
 */
void Camera::placeCamera(){
    if (mode == 0 || mode == 2){
        gluLookAt(this->getPosition().getX(),this->getPosition().getY(),this->getPosition().getZ(),
                  this->getLookAtPosition().getX() ,this->getLookAtPosition().getY(),this->getLookAtPosition().getZ(),
                  this->getUpVector().getX(),this->getUpVector().getY(),this->getUpVector().getZ());
    } else if (mode == 1){
        gluLookAt(this->getPosition().getX(),this->getPosition().getY(),this->getPosition().getZ(),
                  this->getPosition().getX() + this->d.getX(),this->getPosition().getY() + this->d.getY(),this->getPosition().getZ() + this->d.getZ(),
                  this->getUpVector().getX(),this->getUpVector().getY(),this->getUpVector().getZ());
    }
}

/**
 * Returns a string with the camera details
 * @return
 */
const unsigned char *Camera::toString() {
    const unsigned char* s = (const unsigned char*)malloc(10);
    sprintf((char *) s, "MODE: %d", mode);
    return s;
}

/**
 * Calculates alfa based on:
 * - the position the camera is, if in explorer mode
 */
void Camera::calculateAlfa(){
    if (d.getX() == 0 && d.getY() == 0 && d.getZ() == 0){
        if (position.getX() == 0 && position.getZ() == 0) {
            position.setZ(0.01);
        }
        float camPositionY = this->lookAtPosition.getY(); // Projection in the same y-plane as the lookAtPoint
        float zSide = this->position.getZ() -
                      this->lookAtPosition.getZ(); // Length of the side of the triangle parallel to the z axis
        float xSide = this->position.getX() -
                      this->lookAtPosition.getX(); // Length of the side of the triangle parallel to the x axis
        float hip = sqrt(pow(this->position.getX() - this->lookAtPosition.getX(), 2) +
                         pow(this->position.getZ() - this->lookAtPosition.getZ(), 2)); // the hypotenuse

        float accos = acos(zSide / hip); // Angle of Beta (if in the 1st or 2nd quadrants)

        if (xSide < 0) { // Adjust if the angle is in the 3rd or 4th quadrants
            alfa = M_PI - accos + M_PI;
        } else {
            alfa = accos;
        }
    }
    else {
        float div = this->d.getX()/this->d.getZ();
        if (this->d.getZ() < 0){
            alfa = atan(this->d.getX()/this->d.getZ()) + M_PI;
        }
        else alfa = atan(this->d.getX()/this->d.getZ());
    }
}

/**
 * Calculates beta based on:
 * - the position the camera is, if in explorer mode
 */
void Camera::calculateBeta(){
    // r * sin(beta) = y
    // sin(beta) = y/r
    // beta = arcsin(y/r)
    if (d.getX() == 0 && d.getY() == 0 && d.getZ() == 0) {
        float acsin = asin(this->position.getY() / this->cameraRadius);
        beta = acsin; // By default between -PI/2 and PI/2 because of arcsin domain
    }
    else {
        float div = this->d.getY()/this->d.getZ();
        if (this->d.getX() < 0){
            beta = atan(-this->d.getY()/this->d.getZ());
        }
        beta = atan(sqrt(pow(this->d.getX(), 2) + pow(this->d.getY(), 2))/this->d.getZ());
    }
}

void Camera::setPosition(Point position) {
    this->position = position;
}
