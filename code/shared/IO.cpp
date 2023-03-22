//
// Created by mike on 03-03-2023.
//

#include "IO.hpp"
#include <fstream>
#include <string>
#include <math.h>
#include <regex>
#include <sstream>

using namespace std;


/**
 * Generates a file containing all the vertices and faces required to define the respective primitive
 * @param filename the realtive path to the output file
 * @param indexes the vector of points to the respective indexes
 * @param vertices the list of vertices that make up a figure
 */
void writer(string fileName, vector<unsigned int> indexes, vector<float> vertices) {

    ofstream file("../../figures/" + fileName);
    if (!file)
    {
        cout << "Não é possível abrir o ficheiro " << fileName << endl;
    }
    else
    {
        for (int i = 0; i < vertices.size(); i += 3) {
            file << "v " << vertices[i] << " " << vertices[i + 1] << " " << vertices[i + 2] << endl;
        }

        for (int i = 0; i < indexes.size(); i += 3)
        {
            file << "f " << indexes[i] << " " << indexes[i + 1] << " " << indexes[i + 2] << endl;
        }
    }
}

void crossProduct(float vectAX, float vectAY, float vectAZ, float vectBX, float vectBY, float vectBZ, float cross_P[])
{
    cross_P[0] = vectAY * vectBZ - vectAZ * vectBY;
    cross_P[1] = vectAZ * vectBX - vectAX * vectBZ;
    cross_P[2] = vectAX * vectBY - vectAY * vectBX;
}

// Devolve os pontos
vector<Point>* reader(string fileName, vector<Triangle>* triangles){
    ifstream file("../../figures/" + fileName);
    vector<Point> *points = new vector<Point>();
    vector<Point> normals {};
    vector<int> normal_indexes {};

    if (!file)
    {
        cout << "Não é possível abrir o ficheiro " << fileName << endl;
    }
    else
    {
        string line;
        while (getline(file, line, '\n')) {
            // [vf] \d+ \d+ \d+
            vector<string> strings;
            stringstream ss(line);
            string word;
            while (ss >> word) {
                strings.push_back(word);
            }

            if (strings[0] == "v") { // Vertice
                // FORMAT: v x y z

                Point p(stof(strings[1]), stof(strings[2]), stof(strings[3]));
                points->push_back(p);
            }
            else if (strings[0] == "f") {
                // FORMAT: f pointIndex1(/normalIndex1) pointIndex2(/normalIndex2) pointIndex1(/normalIndex2)

                if (strings[1].find("/") == string::npos) {

                    // Get the point indexes
                    int pointInd1 = stoi(strings[1]);
                    int pointInd2 = stoi(strings[2]);
                    int pointInd3 = stoi(strings[3]);
                    Triangle t(pointInd1, pointInd2, pointInd3);
                    triangles->push_back(t);
                }
                else {
                    Triangle* t;

                    size_t f1 = strings[1].find("/");
                    size_t f2 = strings[2].find("/");
                    size_t f3 = strings[3].find("/");

                    int pointInd1 = stoi(strings[1].substr(0, f1));
                    int pointInd2 = stoi(strings[2].substr(0, f2));
                    int pointInd3 = stoi(strings[3].substr(0, f3));    
                    
                    // Get the points
                    Point p1 = (*points)[pointInd1 - 1];
                    Point p2 = (*points)[pointInd2 - 1];
                    Point p3 = (*points)[pointInd3 - 1];

                    int normalIndex = stoi(strings[1].substr(f1+1, strings[1].length()-f1)); // Assume all normal indexes are the same
                    Point normalVector = normals[normalIndex - 1];

                    // From the 3 points, create 2 vectors
                    Point v1(p1.getX() - p2.getX(), p1.getY() - p2.getY(), p1.getZ() - p2.getZ());
                    Point v2(p3.getX() - p2.getX(), p3.getY() - p2.getY(), p3.getZ() - p2.getZ());

                    // Calculate a vector perpendicular to those
                    float cross_P[3];
                    crossProduct(v1.getX(), v1.getY(), v1.getZ(), v2.getX(), v2.getY(), v2.getZ(), cross_P);

                    Point perpendicular(cross_P[0], cross_P[1], cross_P[2]);

                    double norma = sqrt(pow(cross_P[0], 2) + pow(cross_P[1], 2) + pow(cross_P[2], 2)); // Normalization
                    perpendicular.setX(perpendicular.getX() / norma);
                    perpendicular.setY(perpendicular.getY() / norma);
                    perpendicular.setZ(perpendicular.getZ() / norma);

                    // Iguais
                    if (abs(perpendicular.getX() - normalVector.getX()) < 0.1 && abs(perpendicular.getY() - normalVector.getY()) < 0.1 && abs(perpendicular.getZ() - normalVector.getZ()) < 0.1) {
                        t = new Triangle(pointInd1 - 1, pointInd3 - 1, pointInd2 - 1);
                    } // Simétricos
                    else if (abs(perpendicular.getX() + normalVector.getX()) < 0.1 && abs(perpendicular.getY() + normalVector.getY()) < 0.1 && abs(perpendicular.getZ() + normalVector.getZ()) < 0.1) {
                        t = new Triangle(pointInd1 - 1, pointInd2 - 1, pointInd3 - 1);
                    }
                    
                    triangles->push_back(*t);
                }
            }
            else if (strings[0] == "vn"){
                Point p(stof(strings[1]), stof(strings[2]), stof(strings[3]));
                normals.push_back(p);
            }
        }
        return points;
    }
    return NULL;
}