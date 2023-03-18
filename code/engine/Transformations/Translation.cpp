//
// Created by mike on 18-03-2023.
//

#include "Translation.h"

Translation::Translation(float x, float y, float z) : Transformation(x, y, z) {}

string Translation::toString(){
    return "Tranlation";
}