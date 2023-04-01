#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstdlib>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "Transformations/transformation.hpp"
#include "../libraries/tinyxml2.h"
#include "../shared/IO.hpp"
#include "camera.hpp"
#include "group.hpp"


/*float width, height;
Camera* camera;*/

using namespace tinyxml2;

Group::Group() {

}
// Radians
/*
void drawOrbit(float radius, float alpha, float red, float green, float blue, float equation, float radiusLua, float alphaLua, float redLua, float greenLua, float blueLua, float equationLua){
    float posX = radius * cos(alpha);
    float posZ = radius * sin(alpha);
    glPushMatrix();
    glTranslatef(posX, equation*posX, posZ);
    glColor3f(red,green,blue);
    glutSolidSphere(3, 100, 100);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(posX, equation*posX, posZ);
    float luaX = radiusLua * cos(alphaLua);
    float luaZ = radiusLua * sin(alphaLua);
    glTranslatef(luaX, equationLua*luaX, luaZ);
    glColor3f(redLua, greenLua, blueLua);
    glutSolidSphere(1, 100, 100);
    glPopMatrix();

}

void drawEllipticalOrbit(Point center, float a, float b, Point rgb, float alpha){
    // Ellipse equation: (x,y) = (a * cos(alpha), b * sin(alpha))
    // https://en.wikipedia.org/wiki/Ellipse#Standard_parametric_representation
    float posX = a * cos(alpha);
    float posZ = b * sin(alpha);
    glPushMatrix();
    glTranslatef(center.getX(), center.getY(), center.getZ());
    glTranslatef(posX, 0, posZ);
    glColor3f(rgb.getX(), rgb.getY(), rgb.getZ());
    glutSolidSphere(3, 100, 100);
    glPopMatrix();
}*/

/**
 * Draw a triangle, using the order specified by the indexes
 * @param t 3 indexes, specifying the positions of the triangle vertices in the points vector
 * @param red red color setting
 * @param green green color setting
 * @param blue blue color setting
 * @param points the set of all the points in the figure
 */
 /*
void drawTriangle(Triangle t, float red, float green, float blue, vector<Point> points){
    int i1 = t.getIndP1();
    int i2 = t.getIndP2();
    int i3 = t.getIndP3();

    Point p1 = points[i1];
    Point p2 = points[i2];
    Point p3 = points[i3];

    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLES);
    glVertex3f(p1.getX(), p1.getY(), p1.getZ());
    glVertex3f(p2.getX(), p2.getY(), p2.getZ());
    glVertex3f(p3.getX(), p3.getY(), p3.getZ());
    glEnd();
}*/

/**
 * Draw a figure, given the vertices and all the triangles
 * @param triangles list of triangles containing 3 indexes, specifying the positions of the triangle vertices in the points vector
 * @param points the set of all the points in the figure
 * @param red red color setting
 * @param green green color setting
 * @param blue blue color setting
 */
/*
void drawFigure(vector<Triangle> *triangles, vector<Point> *points, float red, float green, float blue){
    int size = triangles->size();
    for(int i=0; i < size; i++){
        drawTriangle((*triangles)[i], red, green, blue, (*points));
    }
}
*/

void Group::drawGroup(float red, float green, float blue) {
    glPushMatrix();

    // Apply the transformations
    for (int i=0; i < this->transformations.size(); i++) {
        this->transformations[i]->applyTransformation();
    }

    for (int i = 0; i < this->vertices.size(); i++) {
        glBindBuffer(GL_ARRAY_BUFFER, this->buffers[i]);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexs[i]);
        glDrawElements(GL_TRIANGLES, this->indexes[i].size(), GL_UNSIGNED_INT, 0);
    }

    for (int i = 0; i < this->subgroups.size(); i++) {
        this->subgroups[i].drawGroup(red, green, blue);
    }

    glPopMatrix();
}

void Group::prepareBuffers() {
    glGenBuffers(this->vertices.size(), this->buffers);
    glGenBuffers(this->indexes.size(), this->indexs);

    for (int i = 0; i < this->vertices.size(); i++) {
        glBindBuffer(GL_ARRAY_BUFFER, this->buffers[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices[i].size(), this->vertices[i].data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->indexes[i].size(), this->indexes[i].data(), GL_STATIC_DRAW);
    }
     
    for (int i = 0; i < this->subgroups.size(); i++) {
        this->subgroups[i].prepareBuffers();
    }

}

void Group::readXML(XMLElement *group) {
    if (group) {

        /* Transformations */
        XMLElement* transformationsElem = group->FirstChildElement("transform");
        if (transformationsElem) {
            for (XMLElement* transform = transformationsElem->FirstChildElement(); transform != NULL; transform = transform->NextSiblingElement()) {
                string tagName = transform->Value();

                float x = stof(transform->Attribute("x"));
                float y = stof(transform->Attribute("y"));
                float z = stof(transform->Attribute("z"));

                if (tagName.compare("translate") == 0){
                    Translation *t = new Translation(x, y, z);
                    this->transformations.push_back(t);
                }
                else if (tagName.compare("rotate") == 0){
                    float angle = stof(transform->Attribute("angle"));
                    Rotation *r = new Rotation(x, y, z,angle);
                    this->transformations.push_back(r);
                }
                else if (tagName.compare("scale") == 0){
                    Scale *s = new Scale(x, y, z);
                    this->transformations.push_back(s);
                }
            }
        }
        int g = 0; // subgroups count
        /* Models */
        XMLElement* models = group->FirstChildElement("models");
        if (models) {
            int i = 0;
            srand(1);
            for (XMLElement* model = models->FirstChildElement(); model != NULL; model = model->NextSiblingElement()) {
                string tagName = model->Value();
                if (tagName.compare("ring") == 0){
                    string fileName = model->Attribute("file");
                    float inner = stof(model->Attribute("inner"));
                    float outer = stof(model->Attribute("outer"));
                    int n = stoi(model->Attribute("n"));
                    float minScale = stof(model->Attribute("minScale"));
                    float maxScale = stof(model->Attribute("maxScale"));
                    float minVAngle = stof(model->Attribute("minVAngle"));
                    float maxVAngle = stof(model->Attribute("maxVAngle"));

                    while (g<n){
                        Group newGroup = Group();
                        newGroup.indexes.push_back(vector<unsigned int>());
                        newGroup.vertices.push_back(reader(fileName, &newGroup.indexes[i]));

                        float angle = ((double)rand() / (double)RAND_MAX) * 360; // Pseudo-random angle between 0 and 360º
                        Rotation* rotation = new Rotation(0, 1, 0, angle);
                        newGroup.transformations.push_back(rotation);

                        float verticalAngle = ((double)rand() / (double)RAND_MAX) * (maxVAngle-minVAngle) + minVAngle; // Pseudo-random angle between 0 and 360º
                        Rotation* verticalRotation = new Rotation(0, 0, 1, verticalAngle);
                        newGroup.transformations.push_back(verticalRotation);

                        float distance = ((double)rand() / (double)RAND_MAX) * (outer - inner) + inner;
                        Translation* translation = new Translation(distance, 0, 0);
                        newGroup.transformations.push_back(translation);

                        float scaleF = ((double)rand() / (double)RAND_MAX) * (maxScale - minScale) + minScale;
                        Scale* scale = new Scale(scaleF, scaleF, scaleF);
                        newGroup.transformations.push_back(scale);

                        newGroup.buffers = new GLuint(i);
                        newGroup.indexs = new GLuint(i);

                        this->subgroups.push_back(newGroup);

                        g++;
                    }
                }
                else if(tagName.compare("model") == 0){
                    this->indexes.push_back(vector<unsigned int>());
                    this->vertices.push_back(reader(model->Attribute("file"), &this->indexes[i]));
                    i++;
                }

            }

            this->buffers = new GLuint(i);
            this->indexs = new GLuint(i);
        }

        /* Groups */
        for (XMLElement* gr = group->FirstChildElement("group"); gr != NULL; gr = gr->NextSiblingElement("group")) {
            this->subgroups.push_back(Group());
            this->subgroups[g].readXML(gr);
            g++;
        }
    }
}
