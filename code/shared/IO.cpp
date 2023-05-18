#include "IO.hpp"
#include <fstream>
#include <string>
#include <regex>
#include <sstream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;


/**
 * Generates a file containing all the vertices and faces required to define the respective primitive
 * @param filename the relative path to the output file
 * @param vertices the list of vertices that make up a figure
 * @param indexes the list of indexes of the points
 * @param normals the list of normals
 * @param textCoord the list of texture coordinates
 */
void writer(const string& fileName, vector<float> vertices, vector<unsigned int> indexes, vector<float> normals, vector<float> textCoord) {
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

        for(int i = 0; i<textCoord.size(); i+=2){
            file << "vt " << textCoord[i] << " " << textCoord[i+1] << endl;
        }

        for (int i = 0; i < normals.size(); i += 3) {
            file << "vn " << normals[i] << " " << normals[i + 1] << " " << normals[i + 2] << endl;
        }

        for (int i = 0; i < indexes.size(); i += 3)
        {
            file << "f " << indexes[i] << " " << indexes[i + 1] << " " << indexes[i + 2] << endl;
        }
    }
}

/*
* Read the primitives files
*/
vector<float> reader(const string& fileName, vector<unsigned int>* indexes, vector<float>* normals, vector<float>* textCoords) {
    ifstream file("../../figures/" + fileName);
    vector<float> vertices;

    if (!file) {
        cout << "Não é possível abrir o ficheiro " << fileName << endl;
    }
    else {
        string line;
        regex r(R"(\d+)");
        regex r1(R"((\d+)\/(\d+)\/(\d+))");

        while (getline(file, line, '\n')) {

            vector<string> strings;
            stringstream ss(line);
            string word;
            while (ss >> word) {
                strings.push_back(word);
            }

            if (strings[0] == "v") {
                // FORMAT: v x y z
                vertices.push_back(stof(strings[1]));
                vertices.push_back(stof(strings[2]));
                vertices.push_back(stof(strings[3]));

            }
            else if (strings[0] == "vt") {
                // FORMAT: vt x y
                textCoords->push_back(stof(strings[1]));
                textCoords->push_back(stof(strings[2]));
            }
            else if (strings[0] == "vn") {
                // FORMAT: vn x y z
                normals->push_back(stof(strings[1]));
                normals->push_back(stof(strings[2]));
                normals->push_back(stof(strings[3]));
            }
            else if (strings[0] == "f") {
                // FORMAT: f vi/ni/ti vi/ni/ti vi/ni/ti
                // FORMAT: f i1 i2 i3
                smatch match;

                for(int i=1; i<=3; i++) {

                    if (regex_search(strings[i], match, r)) {
                        indexes->push_back(stoi(match.str(0)));
                    }
                    else if (regex_search(strings[i], match, r1)) {
                        indexes->push_back(stoi(match.str(1)) - 1);
                        /*int textureIndex = stoi(match.str(2))-1;
                        int normalIndex = stoi(match.str(3))-1;
                        textCoords->push_back(auxTextures[textureIndex * 2]);
                        textCoords->push_back(auxTextures[textureIndex * 2 + 1]);
                        normals->push_back(auxNormals[normalIndex * 3]);
                        normals->push_back(auxNormals[normalIndex * 3 + 1]);
                        normals->push_back(auxNormals[normalIndex * 3 + 2]);*/
                    }
                }
            }
        }

    }
    return vertices;
}

