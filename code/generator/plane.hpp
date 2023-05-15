#include <vector>
#include "../shared/point.hpp"

#ifndef PLANE_H
#define PLANE_H

vector<float> generatePlane(float length, int grid, Point direction, Point initial, bool clockWiseDir, vector<unsigned int> *indexes, int *index, std::vector<float>* normals, std::vector<unsigned int>* normalsIndexes, int *normalIndex);

#endif