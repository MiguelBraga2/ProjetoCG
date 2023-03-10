//
// Created by mike on 03-03-2023.
//

#include "IO.hpp"
#include <fstream>
#include <regex>
#include <string>
#include <math.h>

/**
 * Generates a file containing all the vertices and faces required to define the respective primitive
 * @param filename the realtive path to the output file
 * @param indexes the map of points to the respective indexes
 * @param triangles the list of triangles that make up a figure
 */
void writer(string fileName, map<string, int> indexes, vector<Triangle> triangles) // recebe lista de pontos e os triangulos
{

    vector<pair<string, int> > items;
    for (auto it : indexes) {
        items.push_back(it);
    }
    sort(items.begin(), items.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return a.second < b.second;
    });

    ofstream file("../../figures/" + fileName);
    if (!file)
    {
        cout << "Não é possível abrir o ficheiro " << fileName << endl;
    }
    else
    {
        for (auto it : items) {
            file << "v " << it.first << endl;
        }

        for (auto it : triangles)
        {
            file << "f " << it.getIndP1() << " " << it.getIndP2() << " " << it.getIndP3() << endl;
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
        std::regex regex(R"(^(v|f|vn) (-?\d+\.?\d*).*?(?:\/(\d+))? (-?\d+\.?\d*).*?(?:\/(\d+))? (-?\d+\.?\d*).*?(?:\/(\d+))?$)");
        std::smatch match;

        while (std::getline(file, line, '\n')) {
            // [vf] \d+ \d+ \d+
            if (std::regex_search(line, match, regex)){
                if (match[1].str() == "v"){ // Vertice
                    // FORMAT: v x y z

                    Point p(stof(match[2].str()), stof(match[4].str()), stof(match[6].str()));
                    points->push_back(p);
                }
                else if (match[1].str() == "f"){
                    // FORMAT: f pointIndex1(/normalIndex1) pointIndex2(/normalIndex2) pointIndex1(/normalIndex2)

                    // Get the point indexes
                    int pointInd1 = stoi(match[2].str());
                    int pointInd2 = stoi(match[4].str());
                    int pointInd3 = stoi(match[6].str());

                    Triangle* t;

                    if (match[3].matched == true){
                        // Get the points
                        Point p1 = (*points)[pointInd1-1];
                        Point p2 = (*points)[pointInd2-1];
                        Point p3 = (*points)[pointInd3-1];

                        int normalIndex = stoi(match[3].str()); // Assume all normal indexes are the same
                        Point normalVector = normals[normalIndex-1];

                        // From the 3 points, create 2 vectors
                        Point v1 (p1.getX()-p2.getX(), p1.getY()-p2.getY(), p1.getZ()-p2.getZ());
                        Point v2 (p3.getX()-p2.getX(), p3.getY()-p2.getY(), p3.getZ()-p2.getZ());

                        // Calculate a vector perpendicular to those
                        float cross_P[3];
                        crossProduct(v1.getX(), v1.getY(), v1.getZ(), v2.getX(), v2.getY(), v2.getZ(), cross_P);

                        Point perpendicular (cross_P[0], cross_P[1], cross_P[2]);

                        double norma = sqrt(pow(cross_P[0], 2) + pow(cross_P[1], 2) + pow(cross_P[2], 2)); // Normalization
                        perpendicular.setX(perpendicular.getX()/norma);
                        perpendicular.setY(perpendicular.getY()/norma);
                        perpendicular.setZ(perpendicular.getZ()/norma);

                        // Iguais
                        if (abs(perpendicular.getX() - normalVector.getX()) < 0.1 && abs(perpendicular.getY() - normalVector.getY()) < 0.1 && abs(perpendicular.getZ() -normalVector.getZ()) < 0.1) {
                            t = new Triangle(pointInd1-1, pointInd3-1, pointInd2-1);
                        } // Simétricos
                        else if (abs(perpendicular.getX() + normalVector.getX()) < 0.1 && abs(perpendicular.getY() + normalVector.getY()) < 0.1 && abs(perpendicular.getZ() + normalVector.getZ()) < 0.1) {
                            t = new Triangle(pointInd1-1, pointInd2-1, pointInd3-1);
                        }
                    } else {
                        t = new Triangle(pointInd1, pointInd2, pointInd3);
                    }
                    triangles->push_back(*t);

                }
                else if (match[1].str() == "vn"){
                    Point p(stof(match[2].str()), stof(match[4].str()), stof(match[6].str()));
                    normals.push_back(p);
                }
            }
        }
        return points;
    }

}