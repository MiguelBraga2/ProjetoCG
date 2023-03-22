#ifndef ENGINE_GROUP_H
#define ENGINE_GROUP_H

#include <vector>
#include "../shared/triangle.hpp"
#include "Transformations/Transformation.h"
#include "../libraries/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Group {
private:
    Group(vector<Transformation> transformations, vector<vector<Point> *> points, vector<vector<Triangle> *> figures,
          vector<Group> subgroups);

    vector<Transformation> transformations;
    vector<vector<Point> *> points;
    vector<vector<Triangle> *> figures;
    vector<Group> subgroups;

public:
    void drawGroup();
    void drawTriangle(Triangle t, float red, float green, float blue, vector<Point> points);
    void drawFigure(vector<Triangle>* triangles, vector<Point>* points, float red, float green, float blue);
    void readXML(XMLElement* group);

    const vector<Transformation> &getTransformations() const;

    const vector<vector<Point> *> &getPoints() const;

    const vector<vector<Triangle> *> &getFigures() const;

    const vector<Group> &getSubgroups() const;

    void setTransformations(const vector<Transformation> &transformations);

    void setPoints(const vector<vector<Point> *> &points);

    void setFigures(const vector<vector<Triangle> *> &figures);

    void setSubgroups(const vector<Group> &subgroups);
};


#endif //ENGINE_GROUP_H
