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
    map<string, tuple<Point, float>> initializeTps(vector<Transformation*> *appliedTransforms);
    void drawGroup(bool vboActive, float *matrix, map<string, tuple<Point, float>> *teleports);
    void readXML(XMLElement* group, vector<string>* keys);
    void freeGroup();

private:
    Point calculateCameraTeleport(vector<Transformation*> appliedTransforms, float* radius);
};


#endif //ENGINE_GROUP_H
