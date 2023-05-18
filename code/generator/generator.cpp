#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <iostream>
#include <vector>

#include "plane.hpp"
#include "box.hpp"
#include "cylinder.hpp"
#include "cone.hpp"
#include "torus.hpp"
#include "sphere.hpp"
#include "patch.hpp"
#include "ring.hpp"
#include "../shared/IO.hpp"

using namespace std;


int main(int argc, char** argv) {
    if (argc > 1) {
        if (strcmp(argv[1], "sphere") == 0) {
            if (argc == 6) {
                vector<unsigned int> indexes;
                vector<float> normals;
                vector<unsigned int> normalsIndexes;

                int index = 0;
                Point p1(0,0,0);
                vector<float> vertices = generateSphere(stof(argv[2]), stoi(argv[3]), stoi(argv[4]), &indexes, &normals, &normalsIndexes, p1, &index);
                //writer(argv[5], vertices, indexes, normals, normalsIndexes);
            }
            else {
                cout << "Sphere: número de argumentos inválido." << endl;
            }
        }
        else if (strcmp(argv[1], "cone") == 0) {
            if (argc == 7) {
                vector<unsigned int> indexes;
                vector<float> normals;
                vector<unsigned int> normalsIndexes;

                vector<float> vertices = generateCone(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi(argv[5]), &indexes, &normals, &normalsIndexes);
                //writer(argv[6], vertices, indexes, normals, normalsIndexes);
            }
            else {
                cout << "Cone: número de argumentos inválido." << endl;
            }
        }
        else if (strcmp(argv[1], "box") == 0) {
            if (argc == 5) {
                vector<unsigned int> indexes;
                vector<float> normals;
                vector<float> textCoord;

                Point start(0,0,0);
                int index = 0;

                vector<float> vertices = generateBox(stof(argv[2]), stoi(argv[3]), start, &indexes, &index, &normals, &textCoord);
                writer(argv[4], vertices, indexes, normals,  textCoord);
            }
            else {
                cout << "Box: número de argumentos inválido." << endl;
            }
        }
        else if (strcmp(argv[1], "plane") == 0) {
            if (argc == 5) {
                vector<unsigned int> indexes;
                vector<float> normals;
                vector<float> textCoord;

                float length = stof(argv[2]);
                int index = 0;

                vector<float> vertices = generatePlane(length, stoi(argv[3]), Point(1, 0, 1), Point(-length / 2, 0, -length / 2), false, &indexes, &index, &normals, &textCoord);
                writer(argv[4], vertices, indexes, normals, textCoord);
            }
            else {
                cout << "Plane: número de argumentos inválido." << endl;
            }
        }
        else if (strcmp(argv[1], "cylinder") == 0) {
            if (argc == 6) {
                vector<unsigned int> indexes;
                vector<float> normals;
                vector<float> texCoords;

                vector<float> vertices = generateCylinder(stof(argv[2]), stof(argv[3]), stoi(argv[4]), &indexes, &normals, &texCoords);
                writer(argv[5], vertices, indexes, normals, texCoords);
            }
            else {
                cout << "Cylinder: número de argumentos inválido." << endl;
            }
        }
        else if (strcmp(argv[1], "torus") == 0) {
            if (argc == 7) { // torus InnerRadius OuterRadius Slices Stacks
                vector<unsigned int> indexes;
                vector<float> normals;
                vector<unsigned int> normalsIndexes;
                vector<float> vertices = generateTorus(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi(argv[5]), &indexes, &normals, &normalsIndexes);
                //writer(argv[6], vertices, indexes, normals, normalsIndexes);
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
                    vector<float> normals;
                    vector<unsigned int> normalsIndexes;
                    vector<float> textCoord;
                    vector<unsigned int> textCoordIndexes;

                    vector<float> vertices = generateRing(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stof(argv[5]),
                                                          stof(argv[6]), stof(argv[7]), stof(argv[8]), &indexes, &normals, &normalsIndexes, &textCoord, &textCoordIndexes,
                                                          &argv[10]);
                    //writer(argv[9], vertices, indexes, normals, normalsIndexes);
                }
            }
            else {
                cout << "Ring: número de argumentos inválido." << endl;
            }
        }
        else if (strcmp(argv[1], "patch") == 0){
            if (argc == 5){
                vector<unsigned int> indexes;
                vector<float> normals;
                vector<unsigned int> normalsIndexes;
                vector<Point> controlPoints = readPatch(argv[2], &indexes);
                vector<unsigned int> figureIndexes;
                vector<float> vertices = generatePatches(controlPoints, indexes, stoi(argv[3]), &figureIndexes, &normals, &normalsIndexes);
                //writer(argv[4], vertices, figureIndexes, normals, normalsIndexes);
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