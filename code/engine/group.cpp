#include <string>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "Transformations/transformation.hpp"
#include "Transformations/scale.hpp"
#include "Transformations/translation.hpp"
#include "Transformations/rotation.hpp"
#include "../libraries/tinyxml2.h"
#include "../shared/IO.hpp"
#include "group.hpp"
#include <tuple>

using namespace tinyxml2;

Group::Group() {
}

/**
 * Calculate where the camera must me teleported in explorer mode (and with which radius) if we want to teleport
 * to the center of this group
 * @param appliedTransfs all the transformations these models are subject to
 * @param radius pointer to the radius we want to calculate
 * @return the center we calculated
 */
Point* Group::calculateCameraTeleport(vector<Transformation*> appliedTransfs, float* radius){
    Point* base = new Point(0,0,0);

    for (int i=appliedTransfs.size()-1 ; i>=0; i--) {
        appliedTransfs[i]->applyTransformationToPoint(base, radius); // Apply the transformation to a point
    }

    return base;
}

/**
 * Calculate the teleport position for all the models present in the group
 * @param appliedTransfs all the transformations these models are subject to
 * @return a map with a label as a key and a tuple containing the center and the radius of the model related of the label
 */
map<string, tuple<Point, float>> Group::initializeTeleporter(vector<Transformation*> *appliedTransfs){
    map<string, tuple<Point, float>> teleports;


    for (int i=0; i < this->transformations.size(); i++) {
        if (appliedTransfs != NULL)
            appliedTransfs->push_back(this->transformations[i]);
    }

    for (int i = 0; i < this->models.size(); i++) {
        if (appliedTransfs != NULL && this->models[i].getLabel().compare("undefined") != 0){
            float radius = 4;
            Point* cameraTeleport = calculateCameraTeleport(*appliedTransfs, &radius);
            teleports[this->models[i].getLabel()] = make_tuple(*cameraTeleport, radius);
        }
    }

    for (int i = 0; i < this->subgroups.size(); i++) {
        map<string, tuple<Point, float>> newTeleports = this->subgroups[i].initializeTeleporter(appliedTransfs);

        for (auto it = newTeleports.begin(); it != newTeleports.end(); ++it) {
            teleports[it->first] = it->second;
        }

        appliedTransfs->clear();
        for (int i=0; i < this->transformations.size(); i++) {
            if (appliedTransfs != NULL)
                appliedTransfs->push_back(this->transformations[i]);
        }
    }
    return teleports;
}

/**
 * Draws all the models in a group, after being applied all the transformations
 */
void Group::drawGroup(int vboMode) {
    glPushMatrix(); // Save the current matrix (because when we leave this group we want to "clean" this group's transformations)

    // Apply the transformations
    for (int i=0; i < this->transformations.size(); i++) {
        this->transformations[i]->applyTransformation();
    }

    for (int i = 0; i < this->models.size(); i++) {
        this->models[i].drawModel();
    }

    // Recursively draw each subgroup with the transformations of this group enabled
    for (int i = 0; i < this->subgroups.size(); i++) {
        this->subgroups[i].drawGroup(vboMode);
    }

    glPopMatrix(); // Restore the transformations
}

/**
 * Read all the attributes of a group given a XMLElement of a group and its children
 * Reads:
 * - the transformations
 * - the models/rings
 * - the subgroups
 * @param group XMLElement of a group and its children
 */
void Group::readXML(XMLElement *group) {
    if (group) {
        /* Transformations */
        XMLElement* transformationsElem = group->FirstChildElement("transform");

        if (transformationsElem) { // Optional: can have no transformations
            int numTranslates=0, numScales=0, numRotates=0; // Control that only one transformation of each type is applied

            for (XMLElement* transform = transformationsElem->FirstChildElement(); transform != NULL; transform = transform->NextSiblingElement()) {
                string tagName = transform->Value();
                float x, y, z;
                const char *strX = transform->Attribute("x");
                const char *strY = transform->Attribute("y");
                const char *strZ = transform->Attribute("z");

                if (strX) {
                    x = stof(strX);
                }
                if (strY) {
                    y = stof(strY);
                }
                if (strZ) {
                    z = stof(strZ);
                }

                if (tagName.compare("translate") == 0 && numTranslates == 0){
                    float time;
                    bool align;
                    int tesselation;
                    vector<Point> controlPoints;

                    if (!strX && !strY && !strZ) {

                        const char* strTime = transform->Attribute("time");
                        const char* strAlign = transform->Attribute("align");
                        const char* strTesselation = transform->Attribute("tesselation");

                        time = stof(strTime);
                        tesselation = strTesselation ? stof(strTesselation) : 100;

                        if (std::strcmp(strAlign, "true") == 0) {
                            align = true;
                        }
                        else if (std::strcmp(strAlign, "false") == 0) {
                            align = false;
                        }

                        // Iterate over the control points
                        for (XMLElement* point = transform->FirstChildElement("point"); point != NULL; point = point->NextSiblingElement("point")){
                            x = stof(point->Attribute("x"));
                            y = stof(point->Attribute("y"));
                            z = stof(point->Attribute("z"));
                            Point p(x, y, z);
                            controlPoints.push_back(p);
                        }

                    }
                    else {
                        time = 0;
                        align = false;
                        tesselation = 0;
                    }
                    Translation *t = new Translation(x, y, z, time, align, tesselation, controlPoints);
                    this->transformations.push_back(t);
                    numTranslates++;
                }
                else if (tagName.compare("rotate") == 0 && numRotates == 0){
                    const char* strAngle = transform->Attribute("angle");
                    const char* strTime = transform->Attribute("time");
                    float angle, time;
                    if (strAngle) {
                        angle = stof(strAngle);
                    }
                    else {
                        angle = 0;
                    }
                    if (strTime) {
                        time = stof(strTime);
                    }
                    else {
                        time = 0;
                    }

                    Rotation *r = new Rotation(x, y, z,angle, 0, time);
                    this->transformations.push_back(r);
                    numRotates++;
                }
                else if (tagName.compare("scale") == 0 && numScales == 0){
                    Scale *s = new Scale(x, y, z);
                    this->transformations.push_back(s);
                    numScales++;
                }
            }
        }
        /* Models */
        XMLElement* models = group->FirstChildElement("models");
        if (models) {
            for (XMLElement* model = models->FirstChildElement("model"); model != NULL; model = model->NextSiblingElement("model")) {

                Model m;
                m.readModel(model->Attribute("file"));
                float red = 1, green = 1, blue = 1;
                if (model->Attribute("red")){
                    red = stof(model->Attribute("red"));
                }
                if (model->Attribute("green")){
                    green = stof(model->Attribute("green"));
                }
                if (model->Attribute("blue")){
                    blue = stof(model->Attribute("blue"));
                }
                tuple <float, float, float> tup = make_tuple(red, green, blue);
                m.setRgb(tup);

                if (model->Attribute("label")){
                    m.setLabel(model->Attribute("label"));
                }
                else {
                    m.setLabel("undefined");
                }

                this->models.push_back(m);
            }
        }

        /* Groups */
        for (XMLElement* gr = group->FirstChildElement("group"); gr != NULL; gr = gr->NextSiblingElement("group")) {
            Group g;
            g.readXML(gr);
            this->subgroups.push_back(g);
        }
    }
    else{
        cout << "Group is NULL" << endl;
    }
}
