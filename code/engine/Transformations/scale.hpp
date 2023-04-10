#ifndef ENGINE_SCALE_H
#define ENGINE_SCALE_H

#include "transformation.hpp"

class Scale : public Transformation {
public:
    Scale(float x, float y, float z);
    void applyTransformation() override;
    void calculateExternalPoint(Point* base) override;
};

#endif //ENGINE_SCALE_H
