#ifndef ENGINE_AABB_H
#define ENGINE_AABB_H

#include "volume.hpp"
#include "../../shared/point.hpp"

class AABB : public Volume {
public:
    AABB(Point* corners);
    AABB();
    void setCorners(Point* corners);
    bool test(float *matrix);
    Point* getCorners();

private:
    Point corners[8];
};


#endif 
