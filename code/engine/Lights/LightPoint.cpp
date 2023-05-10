//
// Created by mike on 10-05-2023.
//

#include "LightPoint.h"

LightPoint::LightPoint(float posX, float posY, float posZ) : posX(posX), posY(posY), posZ(posZ) {}

float LightPoint::getPosX() {
    return posX;
}

float LightPoint::getPosY() {
    return posY;
}

float LightPoint::getPosZ() {
    return posZ;
}
