#include "IO.hpp"
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include "../engine/BoundingVolumes/volume.hpp"
#include "../engine/BoundingVolumes/sphere.hpp"
#include "../engine/BoundingVolumes/AABB.hpp"

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
void  writer(const string& fileName, vector<float> vertices, vector<unsigned int> indexes, vector<float> normals, vector<float> textCoord, float radius, Point center) {
    ofstream file("../../figures/" + fileName);
    if (!file)
    {
        cout << "Não é possível abrir o ficheiro " << fileName << endl;
    }
    else
    {   
        file << "bv " << "sphere" << endl;
        file << "bv " << radius << endl;
        file << "bv " << center.getX() << " " << center.getY() << " " << center.getZ() << endl;
        
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

/**
 * Generates a file containing all the vertices and faces required to define the respective primitive
 * @param filename the relative path to the output file
 * @param vertices the list of vertices that make up a figure
 * @param indexes the list of indexes of the points
 * @param normals the list of normals
 * @param textCoord the list of texture coordinates
 */
void writer(const string& fileName, vector<float> vertices, vector<unsigned int> indexes, vector<float> normals, vector<float> textCoord, Point *corners)  {
    ofstream file("../../figures/" + fileName);
    if (!file)
    {
        cout << "Não é possível abrir o ficheiro " << fileName << endl;
    }
    else
    {
        file << "bv " << "box" << endl;
        
        for (int i = 0; i < 8; i ++) {
            file << "bv " << corners[i].getX() << " " << corners[i].getY() << " " << corners[i].getZ() << endl;
        }
        
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


vector<string> mySplit(const string& s) {
    vector<string> strings;
    stringstream ss(s);
    string word;
    while (ss >> word) {
        strings.push_back(word);
    }
    return strings;
}

/*
* Read the primitives files
*/
vector<float> reader(const string& fileName, vector<unsigned int>* indexes, vector<float>* normals, vector<float>* textCoords, Volume **volume) {
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

            vector<string> strings = mySplit(line);

            if (strings[0] == "bv") {
            
                if (strings[1] == "sphere") {
                    /* FORMAT : bv sphere 
                                bv radius 
                                bv x y z */
                    Sphere *v = new Sphere();

                    getline(file, line, '\n');
                    vector<string> strings1 = mySplit(line);
                    v->setRadius(stof(strings1[1]));
                    getline(file, line, '\n');
                    vector<string> strings2 = mySplit(line);
                    v->setCenter(Point(stof(strings2[1]), stof(strings2[2]), stof(strings2[3])));

                    *volume = v; 
                    
                } else if (strings[1] == "box") {
                     /* FORMAT : bv box
                                 bv x y z  (x8)*/ 
                    AABB *v = new AABB();
                    Point corners[8];
                    for (int i = 0; i < 8; i++) {
                        getline(file, line, '\n');
                        vector<string> strings1 = mySplit(line);
                        corners[i].setPoint(stof(strings1[1]),stof(strings1[2]),stof(strings1[3]));
                    }
                    v->setCorners(corners);
                    
                    *volume = v; 
                }
            
            }
            else if (strings[0] == "v") {
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
                // FORMAT: f i1 i2 i3
                smatch match;

                for(int i=1; i<=3; i++) {
                    if (regex_search(strings[i], match, r)) {
                        indexes->push_back(stoi(match.str(0)));
                    }
                    else if (regex_search(strings[i], match, r1)) {
                        indexes->push_back(stoi(match.str(1)) - 1);
                    }
                }
            }
        }

    }
    return vertices;
}

