#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include "ring.hpp"
#include "sphere.hpp"
#include "box.hpp"
#include <string>

vector<float> generateRing (float outerRadius, float innerRadius, int n, float minScale, float maxScale, float minAngle, float maxAngle, vector<unsigned int>* indexes, vector<float>* normals, vector<unsigned int>* normalIndexes, vector<float>* textCoord, vector<unsigned int>* textCoordIndexes, char** args){
    vector<float> vertices;
    vector<float> aux;
    int index = 1;
    int normalIndex = 1;
    int textCoordIndex = 1;

    // For each object to be generated in a ring
    for(int j = 0; j < n; j++) {

        float angle = (float) ((float) rand() / (float) RAND_MAX) * 2 * (float) M_PI; // Pseudo-random angle between 0 and 360º
        float verticalAngle = ((float) rand() / (float) RAND_MAX) * (maxAngle - minAngle) +
                              minAngle; // Pseudo-random angle between 0 and 360º
        verticalAngle = verticalAngle * (float) M_PI / 180;
        float distance = ((float) rand() / (float) RAND_MAX) * (outerRadius - innerRadius) + innerRadius;
        float scaleF = ((float) rand() / (float) RAND_MAX) * (maxScale - minScale) + minScale;

        Point start(distance * cos(verticalAngle) * sin(angle), distance * sin(verticalAngle), distance * cos(verticalAngle) * cos(angle));

        if (strcmp(args[0], "sphere") == 0) {
            aux = generateSphere(scaleF, stoi(args[1]), stoi(args[2]), indexes, normals, normalIndexes, start, &index);
        } else if (strcmp(args[0], "box") == 0) {
            aux = generateBox(scaleF, stoi(args[1]), indexes, start, &index, normals, normalIndexes, &normalIndex, textCoord, textCoordIndexes, &textCoordIndex);
        }
        vertices.insert(vertices.end(), aux.begin(), aux.end());
    }

    return vertices;
}
