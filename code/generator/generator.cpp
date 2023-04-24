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