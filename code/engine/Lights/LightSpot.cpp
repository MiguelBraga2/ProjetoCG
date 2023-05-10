//
// Created by mike on 10-05-2023.
//

#include "LightSpot.h"

LightSpot::LightSpot(float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float cutoff) : posX(posX),
                                                                                                             posY(posY),
                                                                                                             posZ(posZ),
                                                                                                             dirX(dirX),
                                                                                                             dirY(dirY),
                                                                                                             dirZ(dirZ),
                                                                                                             cutoff(cutoff) {}

float LightSpot::getPosX() {
    return posX;
}

float LightSpot::getPosY() {
    return posY;
}

float LightSpot::getPosZ() {
    return posZ;
}

float LightSpot::getDirX() {
    return dirX;
}

float LightSpot::getDirY() {
    return dirY;
}

float LightSpot::getDirZ() {
    return dirZ;
}

float LightSpot::getCutoff() {
    return cutoff;
}

