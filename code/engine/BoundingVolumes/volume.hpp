#ifndef ENGINE_VOLUME_H
#define ENGINE_VOLUME_H

#include "../camera.hpp"

class Volume {
public:
    Volume();
    virtual bool test(float *matrix, Camera *camera);
    virtual Volume* clone();
};


#endif
