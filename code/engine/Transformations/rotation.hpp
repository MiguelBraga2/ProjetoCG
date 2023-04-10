#ifndef ENGINE_ROTATION_H
#define ENGINE_ROTATION_H

#include "transformation.hpp"


class Rotation : public Transformation {
private:
    float angle;
public:
    Rotation(float x, float y, float z, float angle);
    float getAngle();
    void setAngle(float angle);
    void applyTransformation() override;
    void calculateExternalPoint(Point* base) override;
};


#endif //ENGINE_ROTATION_H
