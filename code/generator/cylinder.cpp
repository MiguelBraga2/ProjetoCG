#define _USE_MATH_DEFINES
#include <math.h>
#include "cylinder.hpp"

/**
 * Generates a group of triangles, making a cylinder of given radius and height
 * @param radius the radius of the base and top circles of the cylinder
 * @param height the height of the cylinder
 * @param slices vertical divisions of the sphere
 * @return a list of generated triangles
 */
std::vector<float> generateCylinder(float radius, float height, int slices,std::vector<unsigned int>* verticesIndexes, std::vector<float>* normals,  std::vector<unsigned int>* normalsIndexes) {
    std::vector<float> vertices;
    float aux = height / 2;
    double sliceStep = (2 * M_PI) / slices;
    int index = 0;
    int nIndex = 0;

    for (int i = 0; i < slices; i++) {
        /* normal vector for the vertices of the base triangle */
        normals->push_back(0);
        normals->push_back(-1);
        normals->push_back(0);

        /* normals vectors for the vertices of the lateral triangle */
        normals->push_back((float) sin(i * sliceStep));
        normals->push_back(0);
        normals->push_back((float) cos(i * sliceStep));

        normals->push_back((float) sin((i+1) * sliceStep));
        normals->push_back(0);
        normals->push_back((float) cos((i+1) * sliceStep));

        /* normal vector for the vertices of the top triangle */
        normals->push_back(0);
        normals->push_back(1);
        normals->push_back(0);

        /* Base vertices */
        vertices.push_back(0);
        vertices.push_back(-aux);
        vertices.push_back(0);

        vertices.push_back(radius * (float) sin(i * sliceStep));
        vertices.push_back(-aux);
        vertices.push_back(radius * (float) cos(i * sliceStep));

        vertices.push_back(radius * (float)sin((i + 1) * sliceStep));
        vertices.push_back(-aux);
        vertices.push_back(radius * (float) cos((i + 1) * sliceStep));

        /* Top vertices */
        vertices.push_back(radius * (float) sin(i * sliceStep));
        vertices.push_back(aux);
        vertices.push_back(radius * (float) cos(i * sliceStep));

        vertices.push_back(radius * (float) sin((i + 1) * sliceStep));
        vertices.push_back(aux);
        vertices.push_back(radius * (float) cos((i + 1) * sliceStep));

        vertices.push_back(0);
        vertices.push_back(aux);
        vertices.push_back(0);

        /* Indexes */
        verticesIndexes->push_back(index);
        verticesIndexes->push_back(index + 2);
        verticesIndexes->push_back(index + 1);

        normalsIndexes->push_back(nIndex);
        normalsIndexes->push_back(nIndex);
        normalsIndexes->push_back(nIndex);

        verticesIndexes->push_back(index + 1);
        verticesIndexes->push_back(index + 2);
        verticesIndexes->push_back(index + 3);

        normalsIndexes->push_back(nIndex + 1);
        normalsIndexes->push_back(nIndex + 2);
        normalsIndexes->push_back(nIndex + 1);

        verticesIndexes->push_back(index + 2);
        verticesIndexes->push_back(index + 4);
        verticesIndexes->push_back(index + 3);

        normalsIndexes->push_back(nIndex + 1);
        normalsIndexes->push_back(nIndex + 2);
        normalsIndexes->push_back(nIndex + 2);

        verticesIndexes->push_back(index + 3);
        verticesIndexes->push_back(index + 4);
        verticesIndexes->push_back(index + 5);

        normalsIndexes->push_back(nIndex + 3);
        normalsIndexes->push_back(nIndex + 3);
        normalsIndexes->push_back(nIndex + 3);

        index += 6;
        nIndex += 4;
    }

    return vertices;

}