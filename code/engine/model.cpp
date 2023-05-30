#include "model.hpp"
#include "../shared/IO.hpp" // reader function
#include "../shared/matrixOp.hpp"
#include <map>
#include <cmath>
#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

Model::Model() {
    this->texId = 0;
    this->diffuse[0] = 200.0f / 255.0f;
    this->diffuse[1] = 200.0f / 255.0f;
    this->diffuse[2] = 200.0f / 255.0f;
    this->diffuse[3] = 1;

    this->ambient[0] = 50.0f / 255.0f;
    this->ambient[1] = 50.0f / 255.0f;
    this->ambient[2] = 50.0f / 255.0f;
    this->ambient[3] = 1;

    this->specular[0] = 0;
    this->specular[1] = 0;
    this->specular[2] = 0;
    this->specular[3] = 1;

    this->emissive[0] = 0;
    this->emissive[1] = 0;
    this->emissive[2] = 0;
    this->emissive[3] = 1;

    this->shininess = 0;

}

void Model::drawModel(bool vboActive, float *matrix, map<string, tuple<Point, float>> *teleports) {
    if (this->label.compare("undefined") != 0) {
        Point p1(matrix[3], matrix[7], matrix[11]);
        float res[4];
        float p[4] = {2,2,2,1};
        multiplyMatrixVector(matrix, p, res);
        float radius = sqrt((res[0]-matrix[3])*(res[0]-matrix[3]) + (res[1]-matrix[7])*(res[1]-matrix[7]) + (res[2]-matrix[11])*(res[2]-matrix[11]));
        (*teleports)[this->label] = make_tuple(p1, radius);
    }

    glMaterialfv(GL_FRONT, GL_DIFFUSE, this->diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, this->ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, this->specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, this->emissive);
    glMaterialf(GL_FRONT, GL_SHININESS, this->shininess);

    if (vboActive){
        glBindBuffer(GL_ARRAY_BUFFER,this->vertices);
        glVertexPointer(3,GL_FLOAT,0,0);

        glBindBuffer(GL_ARRAY_BUFFER,this->normals);
        glNormalPointer(GL_FLOAT,0,0);
        
        if (texId != 0){
            glBindTexture(GL_TEXTURE_2D,texId);
            glBindBuffer(GL_ARRAY_BUFFER,this->texCoord);
            glTexCoordPointer(2,GL_FLOAT,0,0);
            glBindTexture(GL_TEXTURE_2D,0);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexes);
        glDrawElements(GL_TRIANGLES,this->indexCount,GL_UNSIGNED_INT,0);
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

            //glColor3f(get<0>(this->rgb), get<1>(this->rgb), get<2>(this->rgb));
            glBegin(GL_TRIANGLES);
            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
            glVertex3f(x3, y3, z3);
            glEnd();

            //glColor3f(1,1,1);
        }
    }
}

/**
 * Reads the vertices and indexes from a file
 * @param fileName name of the file
 */
void Model::readModel(string fileName) {
    vector<unsigned int> indexes;
    vector<float> textCoords;
    vector<float> normals;
    this->verticesVector = reader(fileName, &this->indexesVector, &normals, &textCoords);

    glGenBuffers(1, &(this->vertices));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->verticesVector.size(),this->verticesVector.data(),GL_STATIC_DRAW);

    glGenBuffers(1, &(this->indexes));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexes);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int) * this->indexesVector.size(),this->indexesVector.data(),GL_STATIC_DRAW);

    if (normals.size() > 0){
        glGenBuffers(1, &(this->normals));
        glBindBuffer(GL_ARRAY_BUFFER,this->normals);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size() , normals.data(),GL_STATIC_DRAW);
    }

    if (textCoords.size() > 0){
        glGenBuffers(1, &(this->texCoord));
        glBindBuffer(GL_ARRAY_BUFFER, this->texCoord);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textCoords.size(), textCoords.data(), GL_STATIC_DRAW);
    }

    this->indexCount = this->indexesVector.size();
}

string Model::getLabel() {
    return label;
}

void Model::setLabel(string label) {
    this->label = label;
}

void setMaterial() {

};

void Model::setDiffuse(Point diffuse) {
    this->diffuse[0] = diffuse.getX() / 255;
    this->diffuse[1] = diffuse.getY() / 255;
    this->diffuse[2] = diffuse.getZ() / 255;
}

void Model::setAmbient(Point ambient) {
    this->ambient[0] = ambient.getX() / 255;
    this->ambient[1] = ambient.getY() / 255;
    this->ambient[2] = ambient.getZ() / 255;
}

void Model::setSpecular(Point specular) {
    this->specular[0] = specular.getX() / 255;
    this->specular[1] = specular.getY() / 255;
    this->specular[2] = specular.getZ() / 255;
}

void Model::setEmissive(Point emissive) {
    this->emissive[0] = emissive.getX() / 255;
    this->emissive[1] = emissive.getY() / 255;
    this->emissive[2] = emissive.getZ() / 255;
}

Point Model::getDiffuse() {
    return Point(this->diffuse[0], this->diffuse[1], this->diffuse[2]);
}

Point Model::getAmbient() {
    return Point(this->ambient[0], this->ambient[1], this->ambient[2]);
}

Point Model::getSpecular() {
    return Point(this->specular[0], this->specular[1], this->specular[2]);
}

float Model::getShininess() {
    return shininess;
}

void Model::setShininess(float shininess) {
    this->shininess = shininess;
}

tuple<float, float, float> Model::getRgb(){
    return rgb;
}

void Model::setRgb(tuple<float, float, float> rgb) {
    this->rgb = rgb;
}

void Model::setTextureFile(string textureFile) {
    loadImage(textureFile);
}

void Model::loadImage(string textFile) {
    unsigned int t,tw,th;
    unsigned char *texData;

    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilGenImages(1,&t);
    ilBindImage(t);
    ilLoadImage((ILstring)textFile.c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    glGenTextures(1,&this->texId);

    glBindTexture(GL_TEXTURE_2D,texId);
    glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,		GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,		GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MAG_FILTER,   	GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}


