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
#include <map>

#endif
#include "../shared/point.hpp"


using namespace std;

class Model {
private:
    string label;
    tuple<float, float, float> rgb;
    vector<float> verticesVector;
    vector<unsigned int> indexesVector;
    GLuint vertices;
    GLuint indexes;
    GLuint normals;
    GLuint texCoord;
    int indexCount;
    // Material colors
    float diffuse[4];
    float ambient[4];
    float specular[4];
    float emissive[4];
    float shininess;
    GLuint texId;

public:
    Model();
    void readModel(string fileName);
    void drawModel(bool vboActive, float *matrix, map<string, tuple<Point, float>> *teleports);
    string getLabel();
    void setLabel(string label);
    tuple<float, float, float> getRgb();
    void setRgb(tuple<float, float, float> rgb);
    Point getDiffuse();
    void setDiffuse(Point diffuse);
    Point getAmbient();
    void setAmbient(Point ambient);
    Point getSpecular();
    void setSpecular(Point specular);
    Point getEmissive();
    void setEmissive(Point emissive);
    float getShininess();
    void setShininess(float shininess);
    string getTextureFile();
    void setTextureFile(string textureFile);
private:
    void loadImage(string textFile);
};


#endif //ENGINE_MODEL_H
