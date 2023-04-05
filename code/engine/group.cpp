#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstdlib>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <tuple>

#endif

#include "Transformations/transformation.hpp"
#include "../libraries/tinyxml2.h"
#include "../shared/IO.hpp"
#include "camera.hpp"
#include "group.hpp"

using namespace tinyxml2;

Group::Group() {

}

void Group::drawGroup(float red, float green, float blue) {
    glPushMatrix();

    // Apply the transformations
    for (int i=0; i < this->transformations.size(); i++) {
        this->transformations[i]->applyTransformation();
    }

    for (int i = 0; i < this->vertices.size(); i++) {
        tuple<float, float, float> rgb = this->colors[i];
        glColor3f(get<0>(rgb), get<1>(rgb), get<2>(rgb));
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
            XMLElement* translate = transformationsElem->FirstChildElement("translate");
            if(translate) {
                Translation *t = new Translation(stof(translate->Attribute("x")), stof(translate->Attribute("y")),
                                                 stof(translate->Attribute("z")));
                this->transformations.push_back(t);
            }

            XMLElement* rotate = transformationsElem->FirstChildElement("rotate");
            if(rotate) {
                Rotation *r = new Rotation(stof(rotate->Attribute("x")), stof(rotate->Attribute("y")),
                                           stof(rotate->Attribute("z")), stof(rotate->Attribute("angle")));
                this->transformations.push_back(r);
            }

            XMLElement* scale = transformationsElem->FirstChildElement("scale");
            if (scale) {
                Scale *s = new Scale(stof(scale->Attribute("x")), stof(scale->Attribute("y")),
                                           stof(scale->Attribute("z")));
                this->transformations.push_back(s);
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

                    tuple <float, float, float> tup = make_tuple(stof(model->Attribute("red")), stof(model->Attribute("green")), stof(model->Attribute("blue")));

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

                        newGroup.colors.push_back(tup);

                        newGroup.buffers = new GLuint(i);
                        newGroup.indexs = new GLuint(i);

                        this->subgroups.push_back(newGroup);

                        g++;
                    }
                }
                else if(tagName.compare("model") == 0){
                    this->indexes.push_back(vector<unsigned int>());
                    this->vertices.push_back(reader(model->Attribute("file"), &this->indexes[i]));
                    tuple <float, float, float> tup = make_tuple(stof(model->Attribute("red")), stof(model->Attribute("green")), stof(model->Attribute("blue")));
                    this->colors.push_back(tup);
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
