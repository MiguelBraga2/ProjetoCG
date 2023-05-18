#ifndef IO_HPP
#define IO_HPP

#include <vector>
#include "triangle.hpp"

void writer(const string& fileName, vector<float> vertices, vector<unsigned int> indexes, vector<float> normals, vector<float> textCoord);
vector<float> reader(const string& fileName, vector<unsigned int>* indexes, vector<float>* normals, vector<float>* textCoords);

#endif
