#ifdef __APPLE__
#include <GLUT/glut.h>
#define __GLUT__
#else
#include <GL/glut.h>
#endif

#include "Group.h"
#include "Transformations/Transformation.hpp"
#include "../libraries/tinyxml2.h"
#include "camera.hpp"
#include "../shared/IO.hpp"
#include <iostream>
#include <utility>
#include <stdlib.h>

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
    for (int i=0; i<this->transformations.size(); i++) {
        Transformation* t = this->transformations[i];
        t->applyTransformation();
    }

    glutWireTeapot(2);
    /*
    for (int i=0; i<this->points.size(); i++) {
        this->drawFigure(this->figures[i], this->points[i], red, green, blue);
    }

    for(int i=0; i<this->subgroups.size(); i++){
        Group g = (this->subgroups)[i];
        g.drawGroup(red, green, blue);
    }*/

    glPopMatrix();
}

void Group::readXML(XMLElement *group) {
    // group
    if (group) {
        this->transformations = vector<Transformation*>();
        XMLElement* transformationsElem = group->FirstChildElement("transform");
        if (transformationsElem) {
            for (XMLElement* transform = transformationsElem->FirstChildElement(); transform != NULL; transform = transform->NextSiblingElement()) {
                string tagName = transform->Value();

                float x = stof(transform->Attribute("x"));
                float y = stof(transform->Attribute("y"));
                float z = stof(transform->Attribute("z"));

                if (tagName.compare("translate") == 0){
                    Translation* t = new Translation(x, y, z);
                    this->transformations.push_back(t);
                }
                else if (tagName.compare("rotate") == 0){
                    float angle = stof(transform->Attribute("angle"));
                    Rotation* t = new Rotation(x, y, z,angle);
                    this->transformations.push_back(t);
                }
                else if (tagName.compare("scale") == 0){
                    Scale* t = new Scale(x, y, z);
                    this->transformations.push_back(t);
                }
            }
        }

        /*XMLElement* models = group->FirstChildElement("models");
        if (models) {
            vector<string> *primitives = NULL;

            this->figures = vector<vector<Triangle> *>();
            this->points = vector<vector<Point> *>();

            for (XMLElement* model = models->FirstChildElement("model"); model != NULL; model = model->NextSiblingElement("model")) {

                primitives->emplace_back(model->Attribute("file"));

                int size = primitives->size();
                for (int i = 0; i < size; i++) {
                    this->figures.emplace_back(new vector<Triangle>());
                    this->points.emplace_back(reader((*primitives)[i], (figures)[i]));
                }

                primitives->clear();
                delete primitives;
            }
        }

        XMLElement* subGroup = group->FirstChildElement("group");
        if (subGroup) {
            readXML(subGroup);
        }*/
    }
}