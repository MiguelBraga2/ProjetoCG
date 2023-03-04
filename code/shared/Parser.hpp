//
// Created by mike on 03-03-2023.
//

#ifndef ENGINE_PARSER_HPP
#define ENGINE_PARSER_HPP

#include <list>
#include "triangle.hpp"

void writer(list<Point> points, list<Point> triangles, string fileName);
list<Point> reader(string fileName, list<Point>* triangles, list<Point>* normals, list<int> normal_indexes);

#endif //ENGINE_PARSER_HPP
