//
// Created by mike on 12-04-2023.
//

#include "Model.h"
#include "../shared/IO.hpp"
#include <GL/glut.h>

Model::Model() {
    this->indexes = vector<unsigned int>();
}

void Model::readModel(string fileName) {
    this->vertices = (reader(fileName, &this->indexes));
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

Point Model::getCenter(){
    return center;
}

void Model::setCenter(Point center) {
    Model::center = center;
}

float Model::getRadius() {
    return radius;
}

void Model::setRadius(float radius) {
    Model::radius = radius;
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
