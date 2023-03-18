//
// Created by mike on 18-03-2023.
//

#ifndef ENGINE_ROTATION_H
#define ENGINE_ROTATION_H

#include "Translation.h"

class Rotation : public Transformation{
    float angle;

public:
    Rotation(float x, float y, float z, float angle);

    float getAngle() const;

    void setAngle(float angle);

    string toString();
};


#endif //ENGINE_ROTATION_H
