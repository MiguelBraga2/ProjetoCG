#ifndef ENGINE_GROUP_H
#define ENGINE_GROUP_H

#include <vector>
#include "../shared/triangle.hpp"
#include "../libraries/tinyxml2.h"
#include "Transformations/Transformation.hpp"
#include "Transformations/scale.hpp"
#include "Transformations/translation.hpp"
#include "Transformations/rotation.hpp"

using namespace tinyxml2;

class Group {
private:
    vector<Transformation> transformations;
    vector<vector<float>> vertices;
    vector<vector<unsigned int>> indexes;
    vector<Group> subgroups;

public:
    Group();
    void drawGroup(float red, float green, float blue);
    void drawTriangle(Triangle t, float red, float green, float blue, vector<Point> points);
    void drawFigure(vector<Triangle>* triangles, vector<Point>* points, float red, float green, float blue);
    void readXML(XMLElement* group);
};


#endif //ENGINE_GROUP_H
