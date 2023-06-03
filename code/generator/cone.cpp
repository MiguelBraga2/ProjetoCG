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
std::vector<float> generateCone(float radius, float height, int slices, int stacks, std::vector<unsigned int>* indexes, std::vector<float>* normals, std::vector<float>* textCoords) {
    std::vector<float> vertices;
    float sliceStep = 2 * (float) M_PI / (float) slices;
    float stackStep = height / (float) stacks;
    float radiusDec = radius / (float) stacks;
    float deltaX = 1.0f / slices;
    float deltaY = 1.0f / stacks;
    int index = 0;

    for (int i = 0; i < slices; i++) {
        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(0);
        normals->push_back(0);
        normals->push_back(-1);
        normals->push_back(0);
        textCoords->push_back(0.5f);
        textCoords->push_back(0.5f);

        vertices.push_back(radius * sin((float) i * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos((float) i * sliceStep));
        normals->push_back(0);
        normals->push_back(-1);
        normals->push_back(0);
        textCoords->push_back(0.5f + 0.5 * sin(i * sliceStep));
        textCoords->push_back(0.5f + 0.5 * cos(i * sliceStep));

        vertices.push_back(radius * sin((float) (i + 1) * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos((float) (i + 1) * sliceStep));
        normals->push_back(0);
        normals->push_back(-1);
        normals->push_back(0);
        textCoords->push_back(0.5f + 0.5 * sin((i + 1) * sliceStep));
        textCoords->push_back(0.5f + 0.5 * cos((i + 1) * sliceStep));

        indexes->push_back(index);
        indexes->push_back(index + 2);
        indexes->push_back(index + 1);

        Point p1(radius * sin((float) (i-1) * sliceStep), 0, radius * cos((float) (i-1) * sliceStep));
        Point p2(radius * sin((float) (i+1) * sliceStep), 0, radius * cos((float) (i+1) * sliceStep));
        Point p3(radius * sin((float) i * sliceStep), 0, radius * cos((float) i * sliceStep));
        Point p4(0, height, 0);

        Point v1(p2.getX()-p1.getX(), p2.getY()-p1.getY(), p2.getZ()-p1.getZ());
        Point v2(p4.getX()-p3.getX(), p4.getY()-p3.getY(), p4.getZ()-p3.getZ());

        Point n1 = Point::crossProduct(v1, v2);
        n1.normalize();

        p1.setPoint(radius * sin((float) i * sliceStep), 0, radius * cos((float) i * sliceStep));
        p2.setPoint(radius * sin((float) (i+2) * sliceStep), 0, radius * cos((float) (i+2) * sliceStep));
        p3.setPoint(radius * sin((float) (i+1) * sliceStep), 0, radius * cos((float) (i+1) * sliceStep));

        v1.setPoint(p2.getX()-p1.getX(), p2.getY()-p1.getY(), p2.getZ()-p1.getZ());
        v2.setPoint(p4.getX()-p3.getX(), p4.getY()-p3.getY(), p4.getZ()-p3.getZ());

        Point n2 = Point::crossProduct(v1, v2);
        n2.normalize();

        index += 3;

        vertices.push_back(radius * sin((float) i * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos((float) i * sliceStep));
        normals->push_back(n1.getX());
        normals->push_back(n1.getY());
        normals->push_back(n1.getZ());
        textCoords->push_back(i * deltaX);
        textCoords->push_back(0);

        vertices.push_back(radius * sin((float) (i + 1) * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos((float) (i + 1) * sliceStep));
        normals->push_back(n2.getX());
        normals->push_back(n2.getY());
        normals->push_back(n2.getZ());
        textCoords->push_back((i + 1) * deltaX);
        textCoords->push_back(0);

        for (int j = 0; j < stacks - 1; j++) {
            vertices.push_back((radius - (float) (j + 1) * radiusDec) * sin((float) i * sliceStep));
            vertices.push_back((float) (j + 1) * stackStep);
            vertices.push_back((radius - (float) (j + 1) * radiusDec) * cos((float) i * sliceStep));
            normals->push_back(n1.getX());
            normals->push_back(n1.getY());
            normals->push_back(n1.getZ());
            textCoords->push_back(i * deltaX);
            textCoords->push_back((j+1) * deltaY);

            vertices.push_back((radius - (float) (j + 1) * radiusDec) * sin((float) (i + 1) * sliceStep));
            vertices.push_back((float) (j + 1) * stackStep);
            vertices.push_back((radius - (float) (j + 1) * radiusDec) * cos((float) (i + 1) * sliceStep));
            normals->push_back(n2.getX());
            normals->push_back(n2.getY());
            normals->push_back(n2.getZ());
            textCoords->push_back((i+1) * deltaX);
            textCoords->push_back((j+1) * deltaY);

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
        normals->push_back(0);
        normals->push_back(1);
        normals->push_back(0);
        textCoords->push_back((i+0.5f) * deltaX);
        textCoords->push_back(1);

        indexes->push_back(index);
        indexes->push_back(index + 1);
        indexes->push_back(index + 2);

        index += 3;
    }

    return vertices;
}