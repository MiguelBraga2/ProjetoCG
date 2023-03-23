#ifndef ENGINE_TRANSLATION_H
#define ENGINE_TRANSLATION_H

#include "Transformation.hpp"

class Translation : public Transformation{
public:
    Translation(float x, float y, float z);

    void applyTransformation() override;
};


#endif //ENGINE_TRANSLATION_H
