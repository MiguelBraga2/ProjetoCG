#include <vector>
#include "../shared/point.hpp"

#ifndef SPHERE_H
#define SPHERE_H

std::vector<float> generateSphere(float radius, int slices, int stacks, vector<unsigned int> *verticesIndexes, vector<float> *normals, vector<unsigned int> *normalsIndexes, Point start, int *index);

#endif