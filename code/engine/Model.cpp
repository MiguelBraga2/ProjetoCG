#include "Model.h"
#include "../shared/IO.hpp" // reader function

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

Model::Model() {
    this->indexes = vector<unsigned int>();
}

void Model::drawModel() {
    for (int i = 0; i < this->indexes.size(); i+=3) {
        unsigned int ind1=this->indexes[i], ind2=this->indexes[i+1], ind3=this->indexes[i+2];
        float x1, x2, x3, y1, y2, y3, z1, z2, z3;
        x1 = this->vertices[ind1*3];
        x2 = this->vertices[ind2*3];
        x3 = this->vertices[ind3*3];
        y1 = this->vertices[ind1*3+1];
        y2 = this->vertices[ind2*3+1];
        y3 = this->vertices[ind3*3+1];
        z1 = this->vertices[ind1*3 + 2];
        z2 = this->vertices[ind2*3 + 2];
        z3 = this->vertices[ind3*3 + 2];

        glColor3f(get<0>(this->rgb), get<1>(this->rgb), get<2>(this->rgb));
        glBegin(GL_TRIANGLES);
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y2, z2);
        glVertex3f(x3, y3, z3);
        glEnd();
    }
}

/**
 * Reads the vertices and indexes from a file
 * @param fileName name of the file
 */
void Model::readModel(string fileName) {
    this->vertices = reader(fileName, &this->indexes);
}

string Model::getLabel() {
    return label;
}

void Model::setLabel(string label) {
    Model::label = label;
}

tuple<float, float, float> Model::getRgb(){
    return rgb;
}

void Model::setRgb(tuple<float, float, float> rgb) {
    Model::rgb = rgb;
}

vector<float> Model::getVertices() {
    return vertices;
}

void Model::setVertices(vector<float> vertices) {
    Model::vertices = vertices;
}

vector<unsigned int> Model::getIndexes() {
    return indexes;
}

void Model::setIndexes(vector<unsigned int> indexes) {
    Model::indexes = indexes;
}
