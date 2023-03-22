#ifndef ENGINE_TRANSLATION_H
#define ENGINE_TRANSLATION_H


#include "Transformation.h"

class Translation : public Transformation{
public:
    Translation(float x1, float y1, float z1, float x, float y, float z);

    Translation(float x1, float y1, float z1);

    string toString() ;
};


#endif //ENGINE_TRANSLATION_H
