#include "Model.h"
#include "../shared/IO.hpp" // reader function

Model::Model() {
    this->indexes = vector<unsigned int>();
}

/**
 * Reads the vertices and indexes from a file
 * @param fileName name of the file
 */
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
