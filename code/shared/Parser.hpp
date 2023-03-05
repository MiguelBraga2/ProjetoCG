//
// Created by mike on 03-03-2023.
//

#ifndef ENGINE_PARSER_HPP
#define ENGINE_PARSER_HPP

#include <list>
#include <vector>
#include "triangle.hpp"

void writer(string fileName, list<Point> triangles, list<Point> points);
vector<Point> reader(string fileName, vector<Triangle>* triangles, vector<Point>* normals, vector<int>* normal_indexes);

#endif //ENGINE_PARSER_HPP
