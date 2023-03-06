//
// Created by mike on 03-03-2023.
//

#ifndef ENGINE_IO_HPP
#define ENGINE_IO_HPP

#include <vector>
#include <map>
#include "triangle.hpp"

void writer(string fileName, map<string, int> indexes, vector<Triangle> triangles);
vector<Point>* reader(string fileName, vector<Triangle>* triangles);
void crossProduct(float vectAX, float vectAY, float vectAZ, float vectBX, float vectBY, float vectBZ, float cross_P[]);

#endif //ENGINE_IO_HPP
