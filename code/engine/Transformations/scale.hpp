#ifndef ENGINE_SCALE_H
#define ENGINE_SCALE_H

#include "transformation.hpp"

class Scale : public Transformation {
public:
    Scale(float x, float y, float z);
    void applyTransformation() override;
    void applyTransformationToPoint(Point* base, float* radius) override;
};

#endif //ENGINE_SCALE_H
