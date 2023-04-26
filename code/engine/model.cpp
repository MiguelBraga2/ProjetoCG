#include "model.hpp"
#include "../shared/IO.hpp" // reader function

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

Model::Model() {

}

void Model::drawModel() {
    glColor3f(get<0>(this->rgb), get<1>(this->rgb), get<2>(this->rgb));
    glBindBuffer(GL_ARRAY_BUFFER,this->vertices);
    glVertexPointer(3,GL_FLOAT,0,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexes);
    glDrawElements(GL_TRIANGLES,this->indexCount,GL_UNSIGNED_INT,0);
}

/**
 * Reads the vertices and indexes from a file
 * @param fileName name of the file
 */
void Model::readModel(string fileName) {
    vector<unsigned int> indexes;
    vector<float> vertices = reader(fileName, &indexes);

    glGenBuffers(1, &(this->vertices));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(),vertices.data(),GL_STATIC_DRAW);

    glGenBuffers(1, &(this->indexes));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexes);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int) * indexes.size(),indexes.data(),GL_STATIC_DRAW);

    this->indexCount = indexes.size();
}

string Model::getLabel() {
    return label;
}

void Model::setLabel(string label) {
    this->label = label;
}

tuple<float, float, float> Model::getRgb(){
    return rgb;
}

void Model::setRgb(tuple<float, float, float> rgb) {
    this->rgb = rgb;
}


