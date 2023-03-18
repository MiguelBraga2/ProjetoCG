//
// Created by mike on 18-03-2023.
//

#include "Scale.h"

Scale::Scale(float x, float y, float z) : Transformation(x, y, z) {}

string Scale::toString(){
    return "Scale";
}