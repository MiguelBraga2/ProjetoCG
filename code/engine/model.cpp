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

void Model::drawModel(bool vboActive) {
    if (vboActive){
        glColor3f(get<0>(this->rgb), get<1>(this->rgb), get<2>(this->rgb));
        glBindBuffer(GL_ARRAY_BUFFER,this->vertices);
        glVertexPointer(3,GL_FLOAT,0,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexes);
        glDrawElements(GL_TRIANGLES,this->indexCount,GL_UNSIGNED_INT,0);
        glColor3f(1,1,1);
    }
    else{
        for(int i=0; i<this->indexesVector.size(); i+=3){
            unsigned int ind1=this->indexesVector[i], ind2=this->indexesVector[i+1], ind3=this->indexesVector[i+2];
            float x1, x2, x3, y1, y2, y3, z1, z2, z3;
            x1 = this->verticesVector[ind1*3];
            x2 = this->verticesVector[ind2*3];
            x3 = this->verticesVector[ind3*3];
            y1 = this->verticesVector[ind1*3+1];
            y2 = this->verticesVector[ind2*3+1];
            y3 = this->verticesVector[ind3*3+1];
            z1 = this->verticesVector[ind1*3 + 2];
            z2 = this->verticesVector[ind2*3 + 2];
            z3 = this->verticesVector[ind3*3 + 2];

            glColor3f(get<0>(this->rgb), get<1>(this->rgb), get<2>(this->rgb));
            glBegin(GL_TRIANGLES);
            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
            glVertex3f(x3, y3, z3);
            glEnd();

            glColor3f(1,1,1);
        }
    }
}

/**
 * Reads the vertices and indexes from a file
 * @param fileName name of the file
 */
void Model::readModel(string fileName) {
    vector<unsigned int> indexes;
    this->verticesVector = reader(fileName, &this->indexesVector);

    glGenBuffers(1, &(this->vertices));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->verticesVector.size(),this->verticesVector.data(),GL_STATIC_DRAW);

    glGenBuffers(1, &(this->indexes));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexes);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int) * this->indexesVector.size(),this->indexesVector.data(),GL_STATIC_DRAW);

    this->indexCount = this->indexesVector.size();
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


