#include <vector>
#include <string>
#include "../shared/point.hpp"

#ifndef PATCH_H
#define PATCH_H

std::vector<float> generatePatches(vector<Point> controlPoints, std::vector<unsigned int> patchesIndexes, int tesselation, std::vector<unsigned int>* indexes);
vector<Point> readPatch(const std::string& fileName, std::vector<unsigned int>* indexes);

#endif