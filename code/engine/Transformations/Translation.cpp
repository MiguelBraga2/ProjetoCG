#include "Translation.h"

Translation::Translation(float x1, float y1, float z1) : Transformation(x1, y1, z1) {

}

string Translation::toString(){
    return "Tranlation";
}