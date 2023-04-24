#include "IO.hpp"
#include <fstream>
#include <string>
#define _USE_MATH_DEFINES
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
void writer(const string& fileName, vector<unsigned int> indexes, vector<float> vertices) {

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

/*
* Read the primitives files
*/
vector<float> reader(const string& fileName, vector<unsigned int>* indexes) {
    ifstream file("../../figures/" + fileName);
    vector<float> vertices = vector<float>();
    
    vector<Point> normals {};
    vector<int> normal_indexes {};

    if (!file) {
        cout << "Não é possível abrir o ficheiro " << fileName << endl;
    }
    else {
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
                vertices.push_back(stof(strings[1]));
                vertices.push_back(stof(strings[2]));
                vertices.push_back(stof(strings[3]));

            } else if (strings[0] == "f") {
                // FORMAT: f vericeIndex1(/normalIndex1) verticeIndex2(/normalIndex2) verticeIndex3(/normalIndex2)

                if (strings[1].find('/') == string::npos) {
                    // FORMAT : f i1 i2 i3
                    indexes->push_back(stoi(strings[1]));
                    indexes->push_back(stoi(strings[2]));
                    indexes->push_back(stoi(strings[3]));
                }
                else {
                    size_t f1 = strings[1].find('/');
                    f1 = strings[1].find('/', f1+1);
                    size_t f2 = strings[2].find('/');
                    size_t f3 = strings[3].find('/');

                    int pointInd1 = stoi(strings[1].substr(0, f1));
                    int pointInd2 = stoi(strings[2].substr(0, f2));
                    int pointInd3 = stoi(strings[3].substr(0, f3));    
                    
                    // Get the points
                    Point p1(vertices[pointInd1*3 - 3], vertices[pointInd1*3-2], vertices[pointInd1*3-1]);
                    Point p2(vertices[pointInd2*3 - 3], vertices[pointInd2*3-2], vertices[pointInd2*3-1]);
                    Point p3(vertices[pointInd3*3 - 3], vertices[pointInd3*3-2], vertices[pointInd3*3-1]);

                    int normalIndex = stoi(strings[1].substr(f1+1, strings[1].length()-f1)); // Assume all normal indexes are the same
                    Point normalVector = normals[normalIndex - 1];

                    // From the 3 points, create 2 vectors
                    Point v1(p1.getX() - p2.getX(), p1.getY() - p2.getY(), p1.getZ() - p2.getZ());
                    Point v2(p3.getX() - p2.getX(), p3.getY() - p2.getY(), p3.getZ() - p2.getZ());

                    // Calculate a vector perpendicular to those
                    float cross_P[3];
                    crossProduct(v1.getX(), v1.getY(), v1.getZ(), v2.getX(), v2.getY(), v2.getZ(), cross_P);

                    Point perpendicular(cross_P[0], cross_P[1], cross_P[2]);
                    perpendicular.normalize();

                    if (abs(perpendicular.getX() - normalVector.getX()) < 0.1 && abs(perpendicular.getY() - normalVector.getY()) < 0.1 && abs(perpendicular.getZ() - normalVector.getZ()) < 0.1) {
                        // Iguais
                        indexes->push_back(pointInd1 - 1);
                        indexes->push_back(pointInd3 - 1);
                        indexes->push_back(pointInd2 - 1);
                    } else if (abs(perpendicular.getX() + normalVector.getX()) < 0.1 && abs(perpendicular.getY() + normalVector.getY()) < 0.1 && abs(perpendicular.getZ() + normalVector.getZ()) < 0.1) {
                        // Simétricos
                        indexes->push_back(pointInd1 - 1);
                        indexes->push_back(pointInd2 - 1);
                        indexes->push_back(pointInd3 - 1);
                    }
            
                }
            } else if (strings[0] == "vn"){
                Point p(stof(strings[1]), stof(strings[2]), stof(strings[3]));
                normals.push_back(p);
            }
        }
    }
    return vertices;
}