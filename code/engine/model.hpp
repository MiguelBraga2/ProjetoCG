/**
 * A Model is a representation of an object in the world
 * It has:
 *  - a label;
 *  - color;
 *  - vertices and indexes;
 */

#ifndef ENGINE_MODEL_H
#define ENGINE_MODEL_H

#include <string>
#include <tuple>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "../shared/point.hpp"

using namespace std;

class Model {
private:
    string label;
    tuple<float, float, float> rgb;
    GLuint vertices;
    GLuint indexes;
    int indexCount;

public:
    Model();
    void readModel(string fileName);
    void drawModel();
    string getLabel();
    void setLabel(string label);
    tuple<float, float, float> getRgb();
    void setRgb(tuple<float, float, float> rgb);
};


#endif //ENGINE_MODEL_H
