#ifndef IO_HPP
#define IO_HPP

#include <vector>
#include "triangle.hpp"

void writer(const string& fileName, vector<unsigned int> indexes, vector<float> vertices);
void writer(const string& fileName, vector<float> vertices, vector<unsigned int> verticesIndexes, vector<float> normals, vector<unsigned int> normalsIndexes);
vector<float> reader(const string& fileName, vector<unsigned int>* indexes);
vector<float> reader(const string& fileName, vector<unsigned int>* indexes, vector<float>* normals, vector<float>* textCoords);
void crossProduct(float vectAX, float vectAY, float vectAZ, float vectBX, float vectBY, float vectBZ, float cross_P[]);

#endif
