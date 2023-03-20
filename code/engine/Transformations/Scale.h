#ifndef ENGINE_SCALE_H
#define ENGINE_SCALE_H

#include "Transformation.h"

class Scale : public Transformation{
public:
    Scale(float x, float y, float z);
    string toString() override;
};


#endif //ENGINE_SCALE_H
