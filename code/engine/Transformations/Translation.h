#ifndef ENGINE_TRANSLATION_H
#define ENGINE_TRANSLATION_H


#include "Transformation.h"

class Translation : public Transformation{
public:
    Translation(float x, float y, float z);
    string toString() override;
};


#endif //ENGINE_TRANSLATION_H
