#ifndef ENGINE_GROUP_H
#define ENGINE_GROUP_H

#include <vector>
#include "../shared/triangle.hpp"
#include "../libraries/tinyxml2.h"
#include "Transformations/Transformation.hpp"
#include "Transformations/Scale.h"
#include "Transformations/Translation.h"
#include "Transformations/Rotation.h"

using namespace std;
using namespace tinyxml2;

class Group {
private:
    vector<Transformation*> transformations;
    vector<vector<Point> *> points;
    vector<vector<Triangle> *> figures;
    vector<Group> subgroups;

public:
    Group();
    void drawGroup(float red, float green, float blue);
    void drawTriangle(Triangle t, float red, float green, float blue, vector<Point> points);
    void drawFigure(vector<Triangle>* triangles, vector<Point>* points, float red, float green, float blue);
    void readXML(XMLElement* group);
};


#endif //ENGINE_GROUP_H
