#ifndef ENGINE_TRANSLATION_H
#define ENGINE_TRANSLATION_H

#include "transformation.hpp"

class Translation : public Transformation {
private:
    float duration;
    bool align;
public:
    Translation(float x, float y, float z, float duration, bool align);
    void applyTransformation() override;
    void applyTransformationToPoint(Point* base, float* radius) override;
};

#endif //ENGINE_TRANSLATION_H
