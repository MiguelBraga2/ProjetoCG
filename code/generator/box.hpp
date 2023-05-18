#include <vector>

#ifndef BOX_H
#define BOX_H

vector<float> generateBox(float side, int grid, vector<unsigned int> *indexes, Point start, int *index, vector<float>* normals, vector<unsigned int>* normalsIndexes, int *normalIndex, vector<float>* textCoord, vector<unsigned int>* textCoordIndexes, int *textCoordIndex);

#endif