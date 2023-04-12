//
// Created by mike on 12-04-2023.
//

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
    Point center;
    float radius;
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

    Point getCenter();
    void setCenter(Point center);

    float getRadius();
    void setRadius(float radius);
};


#endif //ENGINE_MODEL_H
