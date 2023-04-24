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
std::vector<float> generateCylinder(float radius, float height, int slices,std::vector<unsigned int>* indexes) {
    std::vector<float> vertices;
    float aux = height / 2;
    double sliceStep = (2 * M_PI) / slices;
    int index = 0;

    for (int i = 0; i < slices; i++) {
        vertices.push_back(0);
        vertices.push_back(-aux);
        vertices.push_back(0);

        vertices.push_back(radius * (float) sin(i * sliceStep));
        vertices.push_back(-aux);
        vertices.push_back(radius * (float) cos(i * sliceStep));

        vertices.push_back(radius * (float)sin((i + 1) * sliceStep));
        vertices.push_back(-aux);
        vertices.push_back(radius * (float) cos((i + 1) * sliceStep));

        vertices.push_back(radius * (float) sin(i * sliceStep));
        vertices.push_back(aux);
        vertices.push_back(radius * (float) cos(i * sliceStep));

        vertices.push_back(radius * (float) sin((i + 1) * sliceStep));
        vertices.push_back(aux);
        vertices.push_back(radius * (float) cos((i + 1) * sliceStep));

        vertices.push_back(0);
        vertices.push_back(aux);
        vertices.push_back(0);

        indexes->push_back(index);
        indexes->push_back(index + 2);
        indexes->push_back(index + 1);

        indexes->push_back(index + 1);
        indexes->push_back(index + 2);
        indexes->push_back(index + 3);

        indexes->push_back(index + 2);
        indexes->push_back(index + 4);
        indexes->push_back(index + 3);

        indexes->push_back(index + 3);
        indexes->push_back(index + 4);
        indexes->push_back(index + 5);

        index += 6;
    }

    return vertices;

}