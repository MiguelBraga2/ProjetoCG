#ifndef ENGINE_TRANSLATION_H
#define ENGINE_TRANSLATION_H

#include <vector>
#include "transformation.hpp"

class Translation : public Transformation {
private:
    float duration;
    bool align;
    vector<Point> controlPoints;
public:
    Translation(float x, float y, float z, float duration, bool align, vector<Point> controlPoints);
    void applyTransformation() override;
    void applyTransformationToPoint(Point* base, float* radius) override;
};

#endif //ENGINE_TRANSLATION_H
