#include "plane.hpp"
#include "box.hpp"

vector<float> generateBox(float side, int grid, vector<unsigned int> *indexes, Point start, int *index, std::vector<float>* normals, std::vector<unsigned int>* normalsIndexes, int *normalIndex) {

    vector<float> vertices = generatePlane(side, grid, Point(1, 0, 1), Point(start.getX() - side / 2, start.getY() - side / 2, start.getZ()-side / 2), true, indexes, index, normals, normalsIndexes, normalIndex);
    vector<float> aux = generatePlane(side, grid, Point(1, 0, 1), Point(start.getX() - side / 2, start.getY() + side / 2, start.getZ() - side / 2), false, indexes, index, normals, normalsIndexes, normalIndex);
    vertices.insert(vertices.end(), aux.begin(), aux.end());
    aux = generatePlane(side, grid, Point(0, -1, -1), Point(start.getX() + side / 2, start.getY() + side / 2, start.getZ() + side / 2), false, indexes, index, normals, normalsIndexes, normalIndex);
    vertices.insert(vertices.end(), aux.begin(), aux.end());
    aux = generatePlane(side, grid, Point(0, -1, -1), Point(start.getX() - side / 2, start.getY() + side / 2, start.getZ() + side / 2), true, indexes, index, normals, normalsIndexes, normalIndex);
    vertices.insert(vertices.end(), aux.begin(), aux.end());
    aux = generatePlane(side, grid, Point(-1, -1, 0), Point(start.getX() + side / 2, start.getY() + side / 2, start.getZ() + side / 2), true, indexes, index, normals, normalsIndexes, normalIndex);
    vertices.insert(vertices.end(), aux.begin(), aux.end());
    aux = generatePlane(side, grid, Point(-1, -1, 0), Point(start.getX() + side / 2, start.getY() + side / 2, start.getZ() - side / 2), false, indexes, index, normals, normalsIndexes, normalIndex);
    vertices.insert(vertices.end(), aux.begin(), aux.end());

    return vertices;
}
