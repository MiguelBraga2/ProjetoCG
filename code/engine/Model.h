/**
 * A Model is a representation of an object in the world
 * It has:
 *  - a label;
 *  - color;
 *  - vertices and indexes;
 */

#ifndef ENGINE_MODEL_H
#define ENGINE_MODEL_H

#include <string>
#include <tuple>
#include <vector>

#include "../shared/point.hpp"

using namespace std;

class Model {
private:
    string label;
    tuple<float, float, float> rgb;
    vector<float> vertices;
    vector<unsigned int> indexes;

public:
    Model();
    void readModel(string fileName);

    vector<float> getVertices();
    void setVertices(vector<float> vertices);

    vector<unsigned int> getIndexes();
    void setIndexes(vector<unsigned int> indexes);

    string getLabel();
    void setLabel(string label);

    tuple<float, float, float> getRgb();
    void setRgb(tuple<float, float, float> rgb);
};


#endif //ENGINE_MODEL_H
