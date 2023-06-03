#include "volume.hpp"
#include "../camera.hpp"

Volume::Volume() {

}

bool Volume::test(float *matrix, Camera *camera) {
    return true; 
}

Volume* Volume::clone() {
    Volume *v = nullptr;
    return v;
}