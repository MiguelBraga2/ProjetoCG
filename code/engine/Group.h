#ifndef ENGINE_GROUP_H
#define ENGINE_GROUP_H

#include <vector>
#include "../shared/triangle.hpp"
#include "Transformations/Transformation.h"
using namespace std;

class Group {
private:
    vector<Transformation> transformations;
    //vector<string> models;
    vector<vector<Point> *> points;
    vector<vector<Triangle> *> figures;
    vector<Group> subgroups;

public:
    void drawGroup(float red, float green, float blue);
    void drawTriangle(Triangle t, float red, float green, float blue, vector<Point> points);
    void drawFigure(vector<Triangle>* triangles, vector<Point>* points, float red, float green, float blue);
    void readXML(char* fileName)
};


#endif //ENGINE_GROUP_H
