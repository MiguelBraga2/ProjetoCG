#define _USE_MATH_DEFINES
#include <math.h>
#include "cone.hpp"

/**
 * Generates a group of triangles, making a cone of given radius and height
 * @param radius the radius of the base circle of the cone
 * @param height the height of the cone
 * @param slices vertical divisions of the cone
 * @param stacks vertical divisions of the cone
 * @return a list of generated triangles
 */
std::vector<float> generateCone(float radius, float height, int slices, int stacks, std::vector<unsigned int>* indexes) {
    std::vector<float> vertices;
    float sliceStep = 2 * (float) M_PI / (float) slices;
    float stackStep = height / (float) stacks;
    float radiusDec = radius / (float) stacks;
    int index = 0;

    for (int i = 0; i < slices; i++) {
        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(0);

        vertices.push_back(radius * sin((float) i * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos((float) i * sliceStep));

        vertices.push_back(radius * sin((float) (i + 1) * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos((float) (i + 1) * sliceStep));

        indexes->push_back(index);
        indexes->push_back(index + 2);
        indexes->push_back(index + 1);

        index++;

        for (int j = 0; j < stacks - 1; j++) {
            vertices.push_back((radius - (float) (j + 1) * radiusDec) * sin((float) i * sliceStep));
            vertices.push_back((float) (j + 1) * stackStep);
            vertices.push_back((radius - (float) (j + 1) * radiusDec) * cos((float) i * sliceStep));

            vertices.push_back((radius - (float) (j + 1) * radiusDec) * sin((float) (i + 1) * sliceStep));
            vertices.push_back((float) (j + 1) * stackStep);
            vertices.push_back((radius - (float) (j + 1) * radiusDec) * cos((float) (i + 1) * sliceStep));

            indexes->push_back(index);
            indexes->push_back(index + 1);
            indexes->push_back(index + 2);

            indexes->push_back(index + 1);
            indexes->push_back(index + 3);
            indexes->push_back(index + 2);

            index += 2;
        }

        vertices.push_back(0);
        vertices.push_back(height);
        vertices.push_back(0);

        indexes->push_back(index);
        indexes->push_back(index + 1);
        indexes->push_back(index + 2);

        index += 3;
    }

    return vertices;
}