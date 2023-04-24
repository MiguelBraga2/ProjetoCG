#include <vector>
#include "../shared/point.hpp"

#ifndef SPHERE_H
#define SPHERE_H

std::vector<float> generateSphere(float radius, int slices, int stacks, std::vector<unsigned int> *indexes, Point start, int *index);

#endif