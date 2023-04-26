/*
 * A group is a set of models and subgroups
 * that will have the same set of transformations applied
 */

#ifndef ENGINE_GROUP_H
#define ENGINE_GROUP_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <vector>
#include <map>

#include "../shared/triangle.hpp"
#include "../libraries/tinyxml2.h"
#include "Transformations/transformation.hpp"
#include "model.hpp"

using namespace tinyxml2;

class Group {



private:
    vector<Transformation*> transformations;
    vector<Model> models;
    vector<Group> subgroups;

public:
    Group();
    map<string, tuple<Point, float>> initializeTeleporter(vector<Transformation*> *appliedTransfs);
    void drawGroup(int vboMode);
    void readXML(XMLElement* group);

private:
    Point* calculateCameraTeleport(vector<Transformation*> appliedTransfs, float* radius);
};


#endif //ENGINE_GROUP_H
