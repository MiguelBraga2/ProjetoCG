#ifndef IO_HPP
#define IO_HPP

#include <vector>
#include "triangle.hpp"

void writer(const string& fileName, vector<unsigned int> indexes, vector<float> vertices);
vector<float> reader(const string& fileName, vector<unsigned int>* indexes);
void crossProduct(float vectAX, float vectAY, float vectAZ, float vectBX, float vectBY, float vectBZ, float cross_P[]);

#endif
