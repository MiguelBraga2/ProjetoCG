#ifndef ENGINE_GROUP_H
#define ENGINE_GROUP_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <vector>
#include "../shared/triangle.hpp"
#include "../libraries/tinyxml2.h"
#include "Transformations/transformation.hpp"
#include "Transformations/scale.hpp"
#include "Transformations/translation.hpp"
#include "Transformations/rotation.hpp"

using namespace tinyxml2;

class Group {
private:
    vector<Transformation*> transformations;
    vector<vector<float>> vertices;
    vector<vector<unsigned int>> indexes;
    vector<tuple<float, float, float>> colors;
    GLuint* buffers;
    GLuint* indexs;
    vector<Group> subgroups;

public:
    Group();
    void prepareBuffers();
    void initializeTeleporter(vector<Transformation> *appliedTransfs, vector<Point>* teleports);
    void drawGroup();
    void readXML(XMLElement* group);

private:
    Point* calculateCameraTeleport(vector<Transformation> appliedTransfs);
};


#endif //ENGINE_GROUP_H
