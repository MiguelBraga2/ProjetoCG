//
// Created by mike on 10-05-2023.
//

#include "LightDirectional.h"

LightDirectional::LightDirectional(float dirX, float dirY, float dirZ) : dirX(dirX), dirY(dirY), dirZ(dirZ) {}

float LightDirectional::getDirX() {
    return dirX;
}

float LightDirectional::getDirY() {
    return dirY;
}

float LightDirectional::getDirZ() {
    return dirZ;
}
