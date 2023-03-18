//
// Created by mike on 18-03-2023.
//

#ifndef ENGINE_TRANSFORMATION_H
#define ENGINE_TRANSFORMATION_H

#include <string>

using namespace std;

class Transformation {
private:
    float x, y, z;
public:
    Transformation(float x, float y, float z);
    float getX() const;

    void setX(float x);

    float getY() const;

    void setY(float y);

    float getZ() const;

    void setZ(float z);

    virtual string toString() = 0;
};


#endif //ENGINE_TRANSFORMATION_H
