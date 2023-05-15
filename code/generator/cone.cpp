#define _USE_MATH_DEFINES
#include <math.h>
#include "cone.hpp"
#include "../shared/point.hpp"

/**
 * Generates a group of triangles, making a cone of given radius and height
 * @param radius the radius of the base circle of the cone
 * @param height the height of the cone
 * @param slices vertical divisions of the cone
 * @param stacks vertical divisions of the cone
 * @return a list of generated triangles
 */
std::vector<float> generateCone(float radius, float height, int slices, int stacks, std::vector<unsigned int>* indexes, std::vector<float>* normals, std::vector<unsigned int>* normalsIndexes) {
    std::vector<float> vertices;
    float sliceStep = 2 * (float) M_PI / (float) slices;
    float stackStep = height / (float) stacks;
    float radiusDec = radius / (float) stacks;
    int index = 0;
    int normalIndex = 0;


    for (int i = 0; i < slices; i++) {
        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(0);

        normals->push_back(0);
        normals->push_back(-1);
        normals->push_back(0);

        vertices.push_back(radius * sin((float) i * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos((float) i * sliceStep));

        vertices.push_back(radius * sin((float) (i + 1) * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos((float) (i + 1) * sliceStep));

        indexes->push_back(index);
        indexes->push_back(index + 2);
        indexes->push_back(index + 1);

        Point p1(0, height, 0);
        Point p2(sin((float) i * sliceStep), 0, cos((float) i * sliceStep));
        Point p3(sin((float) (i + 1) * sliceStep), 0, cos((float) (i + 1) * sliceStep));

        Point v1(p1.getX()-p2.getX(), p1.getY()-p2.getY(), p1.getZ()-p2.getZ());
        Point v2(p3.getX()-p2.getX(), p3.getY()-p2.getY(), p3.getZ()-p2.getZ());

        Point n = Point::crossProduct(v2, v1);

        normals->push_back(n.getX());
        normals->push_back(n.getY());
        normals->push_back(n.getZ());

        normalsIndexes->push_back(normalIndex);
        normalsIndexes->push_back(normalIndex);
        normalsIndexes->push_back(normalIndex);

        index++;
        normalIndex++;

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

            normalsIndexes->push_back(normalIndex);
            normalsIndexes->push_back(normalIndex);
            normalsIndexes->push_back(normalIndex);

            indexes->push_back(index + 1);
            indexes->push_back(index + 3);
            indexes->push_back(index + 2);

            normalsIndexes->push_back(normalIndex);
            normalsIndexes->push_back(normalIndex);
            normalsIndexes->push_back(normalIndex);

            index += 2;
        }

        vertices.push_back(0);
        vertices.push_back(height);
        vertices.push_back(0);

        indexes->push_back(index);
        indexes->push_back(index + 1);
        indexes->push_back(index + 2);

        normalsIndexes->push_back(normalIndex);
        normalsIndexes->push_back(normalIndex);
        normalsIndexes->push_back(normalIndex);

        index += 3;

        normalIndex++;
    }

    return vertices;
}