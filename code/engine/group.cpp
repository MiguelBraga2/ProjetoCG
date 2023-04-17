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

Group::Group(vector<Transformation*> t, vector<Model> models, GLuint* buffers, GLuint* indexs, vector<Group> subgroups) {
    for (size_t i = 0; i < t.size(); i++)
    {
        this->transformations.emplace_back(t[i]);
    }

    for (size_t i = 0; i < models.size(); i++)
    {
        this->models.emplace_back(models[i]);
    }

    // Falta inicializar buffers e indexes 

    for (size_t i = 0; i < subgroups.size(); i++)
    {
        this->subgroups.emplace_back(subgroups[i]);
    }
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
        if (vboMode == 0) {
            this->models[i].drawModel();
        }
        else if (vboMode == 1) {
            tuple<float, float, float> rgb = this->models[i].getRgb();
            glColor3f(get<0>(rgb), get<1>(rgb), get<2>(rgb)); // Change color
            glBindBuffer(GL_ARRAY_BUFFER, this->buffers[i]);
            glVertexPointer(3, GL_FLOAT, 0, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexs[i]);
            glDrawElements(GL_TRIANGLES, this->models[i].getIndexes().size(), GL_UNSIGNED_INT, 0);
        }
        
    }

    // Recursively draw each subgroup with the transformations of this group enabled
    for (int i = 0; i < this->subgroups.size(); i++) {
        this->subgroups[i].drawGroup(vboMode);
    }

    glPopMatrix(); // Restore the transformations
}

void Group::prepareBuffers() {
    glGenBuffers(this->models.size(), this->buffers);
    glGenBuffers(this->models.size(), this->indexs);

    for (int i = 0; i < this->models.size(); i++) {
        glBindBuffer(GL_ARRAY_BUFFER, this->buffers[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->models[i].getVertices().size(), this->models[i].getVertices().data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->models[i].getIndexes().size(), this->models[i].getIndexes().data(), GL_STATIC_DRAW);
    }
     
    for (int i = 0; i < this->subgroups.size(); i++) {
        this->subgroups[i].prepareBuffers();
    }

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
                float x = stof(transform->Attribute("x")), y = stof(transform->Attribute("y")), z = stof(transform->Attribute("z"));

                if (tagName.compare("translate") == 0 && numTranslates == 0){
                    this->transformations.push_back(new Translation(x, y, z));
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

                    this->transformations.push_back(new Rotation(x, y, z,angle, 0, time));
                    numRotates++;
                }
                else if (tagName.compare("scale") == 0 && numScales == 0){
                    this->transformations.push_back(new Scale(x, y, z));
                    numScales++;
                }
            }
        }
        /* Models */
        XMLElement* models = group->FirstChildElement("models");
        int g = 0; // subgroups count - each element of a ring is also a subgroup
        if (models) {
            int i = 0; // number of models
            srand(1);
            for (XMLElement* model = models->FirstChildElement(); model != NULL; model = model->NextSiblingElement()) {
                string tagName = model->Value();
                if (tagName.compare("ring") == 0){
                    string fileName = model->Attribute("file");
                    float inner = stof(model->Attribute("inner")), outer = stof(model->Attribute("outer"));
                    int n = stoi(model->Attribute("n"));
                    float minScale = stof(model->Attribute("minScale")), maxScale = stof(model->Attribute("maxScale"));
                    float minVAngle = stof(model->Attribute("minVAngle")), maxVAngle = stof(model->Attribute("maxVAngle"));
                    tuple <float, float, float> tup = make_tuple(stof(model->Attribute("red")), stof(model->Attribute("green")), stof(model->Attribute("blue")));

                    // For each object to be generated in a ring
                    for(int j=0; j<n; j++){
                        Group newGroup = Group(); // Each object is in a different subgroup (because it has different transformations)
                        Model* m = new Model();
                        m->readModel(fileName);

                        float angle = ((double)rand() / (double)RAND_MAX) * 360; // Pseudo-random angle between 0 and 360º
                        newGroup.transformations.push_back(new Rotation(0, 1, 0, angle, 0, 0));

                        float verticalAngle = ((double)rand() / (double)RAND_MAX) * (maxVAngle-minVAngle) + minVAngle; // Pseudo-random angle between 0 and 360º
                        newGroup.transformations.push_back(new Rotation(0, 0, 1, verticalAngle, 0, 0));

                        float distance = ((double)rand() / (double)RAND_MAX) * (outer - inner) + inner;
                        newGroup.transformations.push_back(new Translation(distance, 0, 0));

                        float scaleF = ((double)rand() / (double)RAND_MAX) * (maxScale - minScale) + minScale;
                        newGroup.transformations.push_back(new Scale(scaleF, scaleF, scaleF));

                        m->setRgb(tup);
                        m->setLabel("undefined");

                        newGroup.buffers = new GLuint(i);
                        newGroup.indexs = new GLuint(i);

                        newGroup.addModel(*m);
                        this->subgroups.push_back(newGroup);
                    }
                    g+=n;
                }
                else if(tagName.compare("model") == 0){
                    Model* m = new Model();
                    m->readModel(model->Attribute("file"));
                    float red, green, blue;
                    if (model->Attribute("red") == NULL){
                        red = 1;
                    }
                    else {
                        red = stof(model->Attribute("red"));
                    }
                    if (model->Attribute("green") == NULL){
                        green = 1;
                    }
                    else {
                        green = stof(model->Attribute("green"));
                    }
                    if (model->Attribute("blue") == NULL){
                        blue = 1;
                    }
                    else {
                        blue = stof(model->Attribute("blue"));
                    }
                    tuple <float, float, float> tup = make_tuple(red, green, blue);
                    m->setRgb(tup);

                    string label;
                    if (model->Attribute("label")){
                        label = model->Attribute("label");
                        m->setLabel(label);
                    }
                    else {
                        m->setLabel("undefined");
                    }

                    this->models.push_back(*m);
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
    else{
        cout << "Group is NULL";
    }
}

void Group::addModel(Model m) {
    this->models.push_back(m);
}

void Group::addTransformation(Transformation* t) {
    this->transformations.push_back(t);
}

Group* Group::clone() {
    Group* g = new Group(this->transformations, this->models, this->buffers, this->indexs, this->subgroups);
    return g;
}
