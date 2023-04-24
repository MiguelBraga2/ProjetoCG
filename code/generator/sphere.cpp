#define _USE_MATH_DEFINES
#include <math.h>
#include "sphere.hpp"

/**
 * Generates a group of triangles that combined approximate a sphere with a given radius using slices and stacks
 * The sphere is centered in the origin (0,0,0)
 * @param radius the radius of the sphere
 * @param slices vertical divisions of the sphere
 * @param stacks horizontal divisions of the sphere
 * @return
 */
vector<float> generateSphere(float radius, int slices, int stacks, vector<unsigned int> *indexes, Point start, int *index) {
    vector<float> vertices;

    float sliceStep = 2 * (float) M_PI / (float) slices;
    float stackStep = M_PI / stacks;

    for (int i = 0; i < slices; i++) {
        vertices.push_back(start.getX());
        vertices.push_back(start.getY()-radius);
        vertices.push_back(start.getZ());

        vertices.push_back(start.getX() + radius * (float) cos(-M_PI / 2 + stackStep) * (float) sin((float) i * sliceStep));
        vertices.push_back(start.getY() + radius * (float) sin(-M_PI / 2 + stackStep));
        vertices.push_back(start.getZ() + radius * (float) cos(-M_PI / 2 + stackStep) * (float) cos((float) i * sliceStep));

        vertices.push_back(start.getX() + radius * (float) cos(-M_PI / 2 + stackStep) * (float) sin((float) (i + 1) * sliceStep));
        vertices.push_back(start.getY() + radius * (float) sin(-M_PI / 2 + stackStep));
        vertices.push_back(start.getZ() + radius * (float) cos(-M_PI / 2 + stackStep) * (float) cos((float) (i + 1) * sliceStep));

        indexes->push_back(*index);
        indexes->push_back(*index + 2);
        indexes->push_back(*index + 1);

        (*index)++;

        for (int j = 1; j < stacks - 1; j++) {
            vertices.push_back(start.getX() + radius * (float) cos(-M_PI / 2 + (float) (j + 1) * stackStep) * sin((float) i * sliceStep));
            vertices.push_back(start.getY() + radius * (float) sin(-M_PI / 2 + (float) (j + 1) * stackStep));
            vertices.push_back(start.getZ() + radius * (float) cos(-M_PI / 2 + (float) (j + 1) * stackStep) * cos((float) i * sliceStep));

            vertices.push_back(start.getX() + radius * (float) cos(-M_PI / 2 + (float) (j + 1) * stackStep) * sin((float) (i + 1) * sliceStep));
            vertices.push_back(start.getY() + radius * (float) sin(-M_PI / 2 + (float) (j + 1) * stackStep));
            vertices.push_back(start.getZ() + radius * (float) cos(-M_PI / 2 + (float) (j + 1) * stackStep) * cos((float) (i + 1) * sliceStep));

            indexes->push_back(*index);
            indexes->push_back(*index + 1);
            indexes->push_back(*index + 2);

            indexes->push_back(*index + 1);
            indexes->push_back(*index + 3);
            indexes->push_back(*index + 2);

            (*index) += 2;
        }

        vertices.push_back(start.getX());
        vertices.push_back(start.getY() + radius);
        vertices.push_back(start.getZ());

        indexes->push_back(*index);
        indexes->push_back(*index + 1);
        indexes->push_back(*index + 2);

        (*index) += 3;
    }

    return vertices;
}