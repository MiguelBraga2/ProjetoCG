#ifndef ENGINE_TRANSFORMATION_H
#define ENGINE_TRANSFORMATION_H

#include <string>

using namespace std;

class Transformation{

public:

    float x;
    float y;
    float z;

    Transformation(float x, float y, float z);

    float getX() const;

    float getY() const;

    float getZ() const;

    void setX(float x);

    void setY(float y);

    void setZ(float z);

};

#endif //ENGINE_TRANSFORMATION_H
