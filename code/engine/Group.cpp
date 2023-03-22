#ifdef __APPLE__
#include <GLUT/glut.h>
#define __GLUT__
#else
#include <GL/glut.h>
#endif

#include "Group.h"
#include "Transformations/Scale.h"
#include "Transformations/Translation.h"
#include "Transformations/Rotation.h"
#include "Transformations/Transformation.h"
#include "../libraries/tinyxml2.h"
#include "camera.hpp"
#include "../shared/IO.hpp"
#include <iostream>
#include <utility>

float width, height;
Camera* camera;

using namespace tinyxml2;

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

Group::Group( vector<Transformation> transformations,vector<vector<Point> *> points,vector<vector<Triangle> *> figures,vector<Group> subgroups){
    this->transformations=std::move(transformations);
    this->points=points;
    this->figures=figures;
    this->subgroups=std::move(subgroups);
}

const vector<Transformation> &Group::getTransformations() const {
    return transformations;
}

const vector<vector<Point> *> &Group::getPoints() const {
    return points;
}

const vector<vector<Triangle> *> &Group::getFigures() const {
    return figures;
}

const vector<Group> &Group::getSubgroups() const {
    return subgroups;
}

/**
 * Draw a triangle, using the order specified by the indexes
 * @param t 3 indexes, specifying the positions of the triangle vertices in the points vector
 * @param red red color setting
 * @param green green color setting
 * @param blue blue color setting
 * @param points the set of all the points in the figure
 */
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
}

/**
 * Draw a figure, given the vertices and all the triangles
 * @param triangles list of triangles containing 3 indexes, specifying the positions of the triangle vertices in the points vector
 * @param points the set of all the points in the figure
 * @param red red color setting
 * @param green green color setting
 * @param blue blue color setting
 */
void drawFigure(vector<Triangle> *triangles, vector<Point> *points, float red, float green, float blue){
    int size = triangles->size();
    for(int i=0; i < size; i++){
        drawTriangle((*triangles)[i], red, green, blue, (*points));
    }
}

template<typename Base, typename T>
bool instanceof(const T *ptr) {
    return dynamic_cast<const Base *>(ptr) != NULL;
}


void Group::drawGroup() {
    glPushMatrix();

    // Apply the transformations
    for (int i=0; i<this->transformations.size(); i++) {
        if (instanceof<Scale>(&this->transformations[i])){
            Scale* s = (Scale*) (&this->transformations[i]);
            glScalef(s->getX(), s->getY(), s->getZ());
        }
        else if (instanceof<Translation>(&this->transformations[i])){
            Translation* t = (Translation* ) (&this->transformations[i]);
            glTranslatef(t->getX(), t->getY(), t->getZ());
        }
        else if (instanceof<Rotation>(&this->transformations[i])){
            Rotation* r = (Rotation*) (&this->transformations[i]);
            glRotatef(r->getAngle(), r->getX(), r->getY(), r->getZ());
        }
    }


    for (int i=0; i<this->points.size(); i++) {

    }

    for(int i=0; i<this->subgroups.size(); i++){

    }
    glPopMatrix();
}

void Group::readXML(XMLElement *group) {
    // group
        if (group) {
            this->transformations = vector<Transformation>();
            XMLElement* transformationsElem = group->FirstChildElement("transformations");
            if (transformationsElem) {
                for (XMLElement* transform = transformationsElem->FirstChildElement("translate"); transform != NULL; transform = transform->NextSiblingElement("translate")) {
                    Transformation t = Translation(0, 0, 0, *transform->Attribute("x"), *transform->Attribute("y"),
                                                   *transform->Attribute("z"));
                    this->transformations.push_back(t);
                }
            }
            XMLElement* models = group->FirstChildElement("models");
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
            }
        }
}