#ifndef ENGINE_SCALE_H
#define ENGINE_SCALE_H

#include "Transformation.hpp"

class Scale : public Transformation{
public:
    Scale(float x, float y, float z);

    void applyTransformation() override;
};

#endif //ENGINE_SCALE_H
