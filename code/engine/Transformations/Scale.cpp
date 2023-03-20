#include "Scale.h"

Scale::Scale(float x, float y, float z) : Transformation(x, y, z) {}

string Scale::toString(){
    return "Scale";
}