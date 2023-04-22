#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <sstream>
#include "../shared/point.hpp"
#include "../shared/IO.hpp"

using namespace std;


/**
 * Generates a group of triangles that combine into a grid, making a square (plane) of given length
 * @param length the length of the larger square
 * @param grid number of smaller squares per side
 * @param direction defines the direction of the plane (for each coordinate, 0 - no direction)
 * For example, 0 in the y direction means the plane is parallel to the y=0 plane
 * @param initial the plane to start generating the plane
 * @param clockWiseDir true if direction is set to clockwise, false if direction is set to counterclockwise
 * @return a list of generated triangles
 */
vector<float> generatePlane(float length, int grid, Point direction, Point initial, bool clockWiseDir, vector<unsigned int> *indexes, int *index){
    vector<float> vertices;
    float step = length / grid; // side of each of the smaller squares

    Point base (initial.getX(), initial.getY(), initial.getZ());

    for(int i=0; i<grid; i++) {

        vertices.push_back(base.getX());
        vertices.push_back(base.getY());
        vertices.push_back(base.getZ());

        if (direction.getX() == 0) {
            vertices.push_back(base.getX());
            vertices.push_back(base.getY());
            vertices.push_back(base.getZ() + step * direction.getZ());
        }
        else if (direction.getY() == 0) {
            vertices.push_back(base.getX() + step * direction.getX());
            vertices.push_back(base.getY());
            vertices.push_back(base.getZ());
        }
        else if (direction.getZ() == 0) {
            vertices.push_back(base.getX() + step * direction.getX());
            vertices.push_back(base.getY());
            vertices.push_back(base.getZ());
        }

        for (int j = 0; j < grid; j++) {

            if (direction.getX() == 0) {
                vertices.push_back(base.getX());
                vertices.push_back(base.getY() + step * direction.getY());
                vertices.push_back(base.getZ());
            }
            else if (direction.getY() == 0) {
                vertices.push_back(base.getX());
                vertices.push_back(base.getY());
                vertices.push_back(base.getZ() + step * direction.getZ());
            }
            else if (direction.getZ() == 0) {
                vertices.push_back(base.getX());
                vertices.push_back(base.getY() + step * direction.getY());
                vertices.push_back(base.getZ());
            }

            vertices.push_back(base.getX() + step * direction.getX());
            vertices.push_back(base.getY() + step * direction.getY());
            vertices.push_back(base.getZ() + step * direction.getZ());

            if (clockWiseDir == false) {
                indexes->push_back(*index);
                indexes->push_back((*index) + 3);
                indexes->push_back((*index) + 1);

                indexes->push_back(*index);
                indexes->push_back((*index) + 2);
                indexes->push_back((*index) + 3);
            } else {
                indexes->push_back(*index);
                indexes->push_back((*index) + 1);
                indexes->push_back((*index) + 3);

                indexes->push_back(*index);
                indexes->push_back((*index) + 3);
                indexes->push_back((*index) + 2);
            }

            // Move the base point
            if (direction.getX() == 0) {
                base.setY(base.getY() + step * direction.getY());
            } else if (direction.getY() == 0) {
                base.setZ(base.getZ() + step * direction.getZ());
            } else if (direction.getZ() == 0) {
                base.setY(base.getY() + step * direction.getY());
            }

            (*index) += 2;
        }
        if (direction.getX() == 0) {
            base.setZ(base.getZ() + step * direction.getZ());
            base.setY(initial.getY());
        } else if (direction.getY() == 0) {
            base.setX(base.getX() + step * direction.getX());
            base.setZ(initial.getZ());
        } else if (direction.getZ() == 0) {
            base.setX(base.getX() + step * direction.getX());
            base.setY(initial.getY());
        }

        (*index) += 2;
    }

    return vertices;
}

vector<float> generateBox(float side, int grid, vector<unsigned int> *indexes, Point start, int *index) {

    vector<float> vertices = generatePlane(side, grid, Point(1, 0, 1), Point(start.getX() - side / 2, start.getY() - side / 2, start.getZ()-side / 2), true, indexes, index);
    vector<float> aux = generatePlane(side, grid, Point(1, 0, 1), Point(start.getX() - side / 2, start.getY() + side / 2, start.getZ() - side / 2), false, indexes, index);
    vertices.insert(vertices.end(), aux.begin(), aux.end());
    aux = generatePlane(side, grid, Point(0, -1, -1), Point(start.getX() + side / 2, start.getY() + side / 2, start.getZ() + side / 2), false, indexes, index);
    vertices.insert(vertices.end(), aux.begin(), aux.end());
    aux = generatePlane(side, grid, Point(0, -1, -1), Point(start.getX() - side / 2, start.getY() + side / 2, start.getZ() + side / 2), true, indexes, index);
    vertices.insert(vertices.end(), aux.begin(), aux.end());
    aux = generatePlane(side, grid, Point(-1, -1, 0), Point(start.getX() + side / 2, start.getY() + side / 2, start.getZ() + side / 2), true, indexes, index);
    vertices.insert(vertices.end(), aux.begin(), aux.end());
    aux = generatePlane(side, grid, Point(-1, -1, 0), Point(start.getX() + side / 2, start.getY() + side / 2, start.getZ() - side / 2), false, indexes, index);
    vertices.insert(vertices.end(), aux.begin(), aux.end());

    return vertices;
}


/**
 * Generates a group of triangles, making a cylinder of given radius and height
 * @param radius the radius of the base and top circles of the cylinder
 * @param height the height of the cylinder
 * @param slices vertical divisions of the sphere
 * @return a list of generated triangles
 */
vector<float> generateCylinder(float radius, float height, int slices,vector<unsigned int>* indexes) {
    vector<float> vertices;
    float aux = height / 2;
    double sliceStep = (2 * M_PI) / slices;
    int index = 0;

    for (int i = 0; i < slices; i++) {
        vertices.push_back(0);
        vertices.push_back(-aux);
        vertices.push_back(0);

        vertices.push_back(radius * sin(i * sliceStep));
        vertices.push_back(-aux);
        vertices.push_back(radius * cos(i * sliceStep));

        vertices.push_back(radius * sin((i + 1) * sliceStep));
        vertices.push_back(-aux);
        vertices.push_back(radius * cos((i + 1) * sliceStep));

        vertices.push_back(radius * sin(i * sliceStep));
        vertices.push_back(aux);
        vertices.push_back(radius * cos(i * sliceStep));

        vertices.push_back(radius * sin((i + 1) * sliceStep));
        vertices.push_back(aux);
        vertices.push_back(radius * cos((i + 1) * sliceStep));

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

/**
 * Generates a group of triangles, making a cone of given radius and height
 * @param radius the radius of the base circle of the cone
 * @param height the height of the cone
 * @param slices vertical divisions of the cone
 * @param stacks vertical divisions of the cone
 * @return a list of generated triangles
 */
vector<float> generateCone(float radius, float height, int slices, int stacks, vector<unsigned int>* indexes) {
    vector<float> vertices;
    double sliceStep = 2 * M_PI / slices;
    double stackStep = height / stacks;
    double radiusDec = radius / stacks;
    int index = 0;

    for (int i = 0; i < slices; i++) {
        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(0);

        vertices.push_back(radius * sin(i * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos(i * sliceStep));

        vertices.push_back(radius * sin((i + 1) * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos((i + 1) * sliceStep));

        indexes->push_back(index);
        indexes->push_back(index + 2);
        indexes->push_back(index + 1);

        index++;

        for (int j = 0; j < stacks - 1; j++) {
            vertices.push_back((radius - (j + 1) * radiusDec) * sin(i * sliceStep));
            vertices.push_back((j + 1) * stackStep);
            vertices.push_back((radius - (j + 1) * radiusDec) * cos(i * sliceStep));

            vertices.push_back((radius - (j + 1) * radiusDec) * sin((i + 1) * sliceStep));
            vertices.push_back((j + 1) * stackStep);
            vertices.push_back((radius - (j + 1) * radiusDec) * cos((i + 1) * sliceStep));

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

    float sliceStep = 2 * M_PI / slices;
    float stackStep = M_PI / stacks;

    for (int i = 0; i < slices; i++) {
        vertices.push_back(start.getX());
        vertices.push_back(start.getY()-radius);
        vertices.push_back(start.getZ());

        vertices.push_back(start.getX() + radius * cos(-M_PI / 2 + stackStep) * sin(i * sliceStep));
        vertices.push_back(start.getY() + radius * sin(-M_PI / 2 + stackStep));
        vertices.push_back(start.getZ() + radius * cos(-M_PI / 2 + stackStep) * cos(i * sliceStep));

        vertices.push_back(start.getX() + radius * cos(-M_PI / 2 + stackStep) * sin((i + 1) * sliceStep));
        vertices.push_back(start.getY() + radius * sin(-M_PI / 2 + stackStep));
        vertices.push_back(start.getZ() + radius * cos(-M_PI / 2 + stackStep) * cos((i + 1) * sliceStep));

        indexes->push_back(*index);
        indexes->push_back(*index + 2);
        indexes->push_back(*index + 1);

        (*index)++;

        for (int j = 1; j < stacks - 1; j++) {
            vertices.push_back(start.getX() + radius * cos(-M_PI / 2 + (j + 1) * stackStep) * sin(i * sliceStep));
            vertices.push_back(start.getY() + radius * sin(-M_PI / 2 + (j + 1) * stackStep));
            vertices.push_back(start.getZ() + radius * cos(-M_PI / 2 + (j + 1) * stackStep) * cos(i * sliceStep));

            vertices.push_back(start.getX() + radius * cos(-M_PI / 2 + (j + 1) * stackStep) * sin((i + 1) * sliceStep));
            vertices.push_back(start.getY() + radius * sin(-M_PI / 2 + (j + 1) * stackStep));
            vertices.push_back(start.getZ() + radius * cos(-M_PI / 2 + (j + 1) * stackStep) * cos((i + 1) * sliceStep));

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


vector<float> generateTorus(float innerRadius, float outerRadius, float slices, float stacks, vector<unsigned int>* indexes){
    vector<float> vertices;

    float alpha = 2 * M_PI / slices; // Defines the position around the y axis
    float beta = 2 * M_PI / stacks; // Defines the height
    float radius = (outerRadius - innerRadius) / 2;
    float distanceToOrigin = innerRadius + radius;
    int index = 0;

    for(int i=0; i < slices; i++){ // Percorrer as slices

        Point c1(distanceToOrigin * sin(i * alpha), 0, distanceToOrigin * cos(i * alpha));
        Point c2(distanceToOrigin * sin((i + 1) * alpha), 0, distanceToOrigin * cos((i + 1) * alpha));

        Point p3 (radius * sin(i * alpha), 0, radius * cos(i * alpha));
        Point p4 (radius * sin((i + 1) * alpha), 0, radius * cos((i + 1) * alpha));

        vertices.push_back(c1.getX() + p3.getX());
        vertices.push_back(c1.getY() + p3.getY());
        vertices.push_back(c1.getZ() + p3.getZ());

        vertices.push_back(c2.getX() + p4.getX());
        vertices.push_back(c2.getY() + p4.getY());
        vertices.push_back(c2.getZ() + p4.getZ());

        for(int j = 0; j < stacks; j++){ // Percorrer as stacks

            Point p1 (radius * cos((j + 1) * beta) * sin(i * alpha), radius * sin((j + 1) * beta), radius * cos((j + 1) * beta) * cos(i * alpha));
            Point p2 (radius * cos((j + 1) * beta) * sin((i + 1) * alpha), radius * sin((j + 1) * beta), radius * cos((j + 1) * beta) * cos((i + 1) * alpha));

            vertices.push_back(c1.getX() + p1.getX());
            vertices.push_back(c1.getY() + p1.getY());
            vertices.push_back(c1.getZ() + p1.getZ());

            vertices.push_back(c2.getX() + p2.getX());
            vertices.push_back(c2.getY() + p2.getY());
            vertices.push_back(c2.getZ() + p2.getZ());

            indexes->push_back(index);
            indexes->push_back(index + 1);
            indexes->push_back(index + 2);

            indexes->push_back(index + 1);
            indexes->push_back(index + 3);
            indexes->push_back(index + 2);

            index += 2;
        }

        index += 2;
    }

    return vertices;
}

vector<float> generateRing (float outerRadius, float innerRadius, int n, float minScale, float maxScale, float minAngle, float maxAngle, vector<unsigned int>* indexes, char** args){
    vector<float> vertices;
    vector<float> aux;
    int index = 0;

    // For each object to be generated in a ring
    for(int j = 0; j < n; j++) {

        float angle = ((float) rand() / (float) RAND_MAX) * 2 * M_PI; // Pseudo-random angle between 0 and 360º
        float verticalAngle = ((float) rand() / (float) RAND_MAX) * (maxAngle - minAngle) +
                              minAngle; // Pseudo-random angle between 0 and 360º
        verticalAngle = verticalAngle * M_PI / 180;
        float distance = ((float) rand() / (float) RAND_MAX) * (outerRadius - innerRadius) + innerRadius;
        float scaleF = ((float) rand() / (float) RAND_MAX) * (maxScale - minScale) + minScale;

        Point start(distance * cos(verticalAngle) * sin(angle), distance * sin(verticalAngle), distance * cos(verticalAngle) * cos(angle));

        if (strcmp(args[0], "sphere") == 0) {
            aux = generateSphere(scaleF, stoi(args[1]), stoi(args[2]), indexes, start, &index);
        } else if (strcmp(args[0], "box") == 0) {
            aux = generateBox(scaleF, stoi(args[1]), indexes, start, &index);
        }
        vertices.insert(vertices.end(), aux.begin(), aux.end());
    }

    return vertices;
}

vector<float> generatePatches(vector<Point> patches, vector<unsigned int> patchesIndexes, int tesselation, vector<unsigned int>* indexes) {
    vector<float> generatedPoints;
    int index=0;
    float delta = 1.0 / tesselation;

    // 16 pontos
    float M[4][4] = {{-1, 3, -3, 1},
                     {3, -6, 3, 0},
                     {-3, 3, 0, 0},
                     {1, 0, 0, 0}};

    for(int k=0; k<patchesIndexes.size(); k+=16){

        Point Points[4][4] = {{patches[patchesIndexes[k]], patches[patchesIndexes[k+4]], patches[patchesIndexes[k+8]], patches[patchesIndexes[k+12]]},
                              {patches[patchesIndexes[k+1]], patches[patchesIndexes[k+5]], patches[patchesIndexes[k+9]], patches[patchesIndexes[k+13]]},
                              {patches[patchesIndexes[k+2]], patches[patchesIndexes[k+6]], patches[patchesIndexes[k+10]], patches[patchesIndexes[k+14]]},
                              {patches[patchesIndexes[k+3]], patches[patchesIndexes[k+7]], patches[patchesIndexes[k+11]], patches[patchesIndexes[k+15]]}};

        Point* aux;
        Point* pre;
        Point::multMatrixPointMatrix(*M, 4, 4, *Points, 4, 4, &aux);
        Point::multPointMatrixMatrix(aux, 4, 4, *M, 4, 4, &pre);

        for(int i=0; i<tesselation; i++){
            float u = delta*i;
            float us = delta*(i + 1);
            float u_vector[4] = {u*u*u, u*u, u, 1};
            float uplus1_vector[4] = {(us)*(us)*(us), (us)*(us), us, 1};
            Point* first;
            Point::multMatrixPointMatrix(u_vector, 1, 4, pre, 4, 4, &first);
            Point* second;
            Point::multMatrixPointMatrix(uplus1_vector, 1, 4, pre, 4, 4, &second);
            for(int j=0; j<tesselation; j++){
                float v = delta*j;
                float vs = delta * (j+1);
                Point* P1, *P2, *P3, *P4;
                float v_vector[4] = {v*v*v, v*v, v, 1};
                float vplus1_vecotr[4] = {(vs)*(vs)*(vs), (vs)*(vs), vs, 1};
                Point::multPointMatrixMatrix(first, 4, 4, v_vector, 4, 1, &P1);
                Point::multPointMatrixMatrix(first, 4, 4, vplus1_vecotr, 4, 1, &P2);
                Point::multPointMatrixMatrix(second, 4, 4, v_vector, 4, 1, &P3);
                Point::multPointMatrixMatrix(second, 4, 4, vplus1_vecotr, 4, 1, &P4);
                generatedPoints.push_back(P1[0].getX()); generatedPoints.push_back(P1[0].getY()); generatedPoints.push_back(P1[0].getZ());
                generatedPoints.push_back(P2[0].getX()); generatedPoints.push_back(P2[0].getY()); generatedPoints.push_back(P2[0].getZ());
                generatedPoints.push_back(P3[0].getX()); generatedPoints.push_back(P3[0].getY()); generatedPoints.push_back(P3[0].getZ());
                generatedPoints.push_back(P4[0].getX()); generatedPoints.push_back(P4[0].getY()); generatedPoints.push_back(P4[0].getZ());
                indexes->push_back(index);
                indexes->push_back(index+2);
                indexes->push_back(index+1);
                indexes->push_back(index+2);
                indexes->push_back(index+3);
                indexes->push_back(index+1);
                index+=4;
            }
        }
    }
    return generatedPoints;

}

vector<Point> readPatch(string fileName, vector<unsigned int>* indexes){
    ifstream file("../patches/" + fileName);
    vector<Point> controlPoints;

    if (!file) {
        cout << "Não é possível abrir o ficheiro " << fileName << endl;
    }
    else{
        string line;
        regex r("\\s+");
        getline(file, line); // Read the first line (number of patches)
        line = regex_replace(line, r, "");
        int nPatches = stoi(line);

        for(int i = 0; i < nPatches && getline(file, line); i++) {
            line = regex_replace(line, r, "");

            string first;
            stringstream ss(line);
            for(int j = 0; j < 16 && getline(ss, first, ','); j++) {
                indexes->push_back(stoi(first));
            }
        }

        getline(file, line); // Read the number of points
        line = regex_replace(line, r, "");
        int nPoints = stoi(line);

        regex re(R"(\s*([-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?)\s*,\s*([-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?)\s*,\s*([-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?)\s*)");
        smatch match;

        for(int i = 0; i < nPoints && getline(file, line); i++) {
            if (regex_search(line, match, re) == true) {
                Point p(stof(match.str(1)), stof(match.str(2)), stof(match.str(3)));
                controlPoints.push_back(p);
            }
        }
    }
    file.close();

    return controlPoints;
}

int main(int argc, char** argv) {
    if (argc > 1) {
        if (strcmp(argv[1], "sphere") == 0) {
            if (argc == 6) {
                vector<unsigned int> indexes;
                int index = 0;
                Point p1(0,0,0);
                vector<float> vertices = generateSphere(stof(argv[2]), stoi(argv[3]), stoi(argv[4]), &indexes, p1, &index);
                writer(argv[5], indexes, vertices);
            }
            else {
                cout << "Sphere: número de argumentos inválido." << endl;
            }
        }
        else if (strcmp(argv[1], "cone") == 0) {
            if (argc == 7) {
                vector<unsigned int> indexes;
                vector<float> vertices = generateCone(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi(argv[5]), &indexes);
                writer(argv[6], indexes, vertices);
            }
            else {
                cout << "Cone: número de argumentos inválido." << endl;
            }
        }
        else if (strcmp(argv[1], "box") == 0) {
            if (argc == 5) {
                vector<unsigned int> indexes;
                Point start(0,0,0);
                int index = 0;

                vector<float> vertices = generateBox(stof(argv[2]), stoi(argv[3]), &indexes, start, &index);
                writer(argv[4], indexes, vertices);
            }
            else {
                cout << "Box: número de argumentos inválido." << endl;
            }

        }
        else if (strcmp(argv[1], "plane") == 0) {
            if (argc == 5) {
                vector<unsigned int> indexes;
                float length = stof(argv[2]);
                int index = 0;
                vector<float> vertices = generatePlane(length, stoi(argv[3]), Point(1, 0, 1), Point(-length / 2, 0, -length / 2), false, &indexes, &index);
                writer(argv[4], indexes, vertices);
            }
            else {
                cout << "Plane: número de argumentos inválido." << endl;
            }
        }
        else if (strcmp(argv[1], "cylinder") == 0) {
            if (argc == 6) {
                vector<unsigned int> indexes;
                vector<float> vertices = generateCylinder(stof(argv[2]), stof(argv[3]), stoi(argv[4]), &indexes);
                writer(argv[5], indexes, vertices);
            }
            else {
                cout << "Cylinder: número de argumentos inválido." << endl;
            }
        }
        else if (strcmp(argv[1], "torus") == 0) {
            if (argc == 7) { // torus InnerRadius OuterRadius Slices Stacks
                vector<unsigned int> indexes;
                vector<float> vertices = generateTorus(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi(argv[5]), &indexes);
                writer(argv[6], indexes, vertices);
            }
            else {
                cout << "Torus: número de argumentos inválido." << endl;
            }
        }
        else if (strcmp(argv[1], "ring") == 0) {
            if (argc > 10) {
                if (strcmp(argv[10],"sphere")!=0 && strcmp(argv[10],"box")!=0) {
                    cout << "Primitiva inválida para ser usada como asteroide." << endl;
                } else {
                    vector<unsigned int> indexes;
                    vector<float> vertices = generateRing(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stof(argv[5]),
                                                          stof(argv[6]), stof(argv[7]), stof(argv[8]), &indexes,
                                                          &argv[10]);
                    writer(argv[9], indexes, vertices);
                }
            }
            else {
                cout << "Ring: número de argumentos inválido." << endl;
            }
        }
        else if (strcmp(argv[1], "patch") == 0){
            /* generator patch teapot.patch 10 bezier_10.3d */
            if (argc == 5){
                vector<unsigned int> indexes;
                vector<Point> controlPoints = readPatch(argv[2], &indexes);
                vector<unsigned int> figureIndexes;
                vector<float> vertices = generatePatches(controlPoints, indexes, stoi(argv[3]), &figureIndexes);
                writer(argv[4], figureIndexes, vertices);
            }
            else {
                cout << "Patch: número de argumentos inválido." << endl;
            }
        }
        else {
            cout << "Figura desconhecida." << endl;
        }
    }
    else {
        cout << "Não existem argumentos a serem passados." << endl;
    }
}