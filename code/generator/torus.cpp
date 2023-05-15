#define _USE_MATH_DEFINES
#include <math.h>
#include "torus.hpp"
#include "../shared/point.hpp"

std::vector<float> generateTorus(float innerRadius, float outerRadius, int slices, int stacks, std::vector<unsigned int>* indexes, std::vector<float>* normals, std::vector<unsigned int>* normalsIndexes){
    std::vector<float> vertices;

    float alpha = 2 * (float) M_PI / (float) slices; // Defines the position around the y-axis
    float beta = 2 * (float) M_PI / (float) stacks; // Defines the height
    float radius = (outerRadius - innerRadius) / 2;
    float distanceToOrigin = innerRadius + radius;
    int index = 0;

    for(int i=0; i < slices; i++){ // Percorrer as slices

        Point c1(distanceToOrigin * sin((float) i * alpha), 0, distanceToOrigin * cos((float) i * alpha));
        Point c2(distanceToOrigin * sin((float) (i + 1) * alpha), 0, distanceToOrigin * cos((float) (i + 1) * alpha));

        Point p3 (radius * sin((float) i * alpha), 0, radius * cos((float) i * alpha));
        Point p4 (radius * sin((float) (i + 1) * alpha), 0, radius * cos((float) (i + 1) * alpha));

        vertices.push_back(c1.getX() + p3.getX());
        vertices.push_back(c1.getY() + p3.getY());
        vertices.push_back(c1.getZ() + p3.getZ());

        vertices.push_back(c2.getX() + p4.getX());
        vertices.push_back(c2.getY() + p4.getY());
        vertices.push_back(c2.getZ() + p4.getZ());

        normals->push_back(sin((float) i * alpha));
        normals->push_back(0);
        normals->push_back(cos((float) i * alpha));

        normals->push_back(sin((float) (i + 1) * alpha));
        normals->push_back(0);
        normals->push_back(cos((float) (i + 1) * alpha));


        for(int j = 0; j < stacks; j++){

            Point p1 (radius * cos((float) (j + 1) * beta) * sin((float) i * alpha), radius * sin((float) (j + 1) * beta), radius * cos((float) (j + 1) * beta) * cos((float) i * alpha));
            Point p2 (radius * cos((float) (j + 1) * beta) * sin((float) (i + 1) * alpha), radius * sin((float) (j + 1) * beta), radius * cos((float) (j + 1) * beta) * cos((float) (i + 1) * alpha));

            vertices.push_back(c1.getX() + p1.getX());
            vertices.push_back(c1.getY() + p1.getY());
            vertices.push_back(c1.getZ() + p1.getZ());

            vertices.push_back(c2.getX() + p2.getX());
            vertices.push_back(c2.getY() + p2.getY());
            vertices.push_back(c2.getZ() + p2.getZ());

            normals->push_back(cos((float) (j + 1) * beta) * sin((float) i * alpha));
            normals->push_back(sin((float) (j + 1) * beta));
            normals->push_back(cos((float) (j + 1) * beta) * cos((float) i * alpha));

            normals->push_back(cos((float) (j + 1) * beta) * sin((float) (i + 1) * alpha));
            normals->push_back(sin((float) (j + 1) * beta));
            normals->push_back(cos((float) (j + 1) * beta) * cos((float) (i + 1) * alpha));

            indexes->push_back(index);
            indexes->push_back(index + 1);
            indexes->push_back(index + 2);

            normalsIndexes->push_back(index);
            normalsIndexes->push_back(index + 1);
            normalsIndexes->push_back(index + 2);

            indexes->push_back(index + 1);
            indexes->push_back(index + 3);
            indexes->push_back(index + 2);

            normalsIndexes->push_back(index + 1);
            normalsIndexes->push_back(index + 3);
            normalsIndexes->push_back(index + 2);

            index += 2;
        }

        index += 2;
    }

    return vertices;
}