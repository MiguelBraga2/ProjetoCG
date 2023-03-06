//
// Created by mike on 03-03-2023.
//

#include "Parser.hpp"
#include <fstream>
#include <regex>
#include <string>
#include <math.h>

list<Triangle> copyList(list<Triangle> original){
    list<Triangle> newList {};
    std::list<Triangle>::iterator it;

    for (it = original.begin(); it != original.end(); ++it){
        newList.push_front(it.operator*());
    }

    return newList;
}

bool comparator (const Point& first, const Point& second){
    return false;
}

void writer(string fileName, list<Point> triangles, list<Point> points){
    ofstream file("../" + fileName);
    if (!file)
    {
        cout << "Não é possível abrir o ficheiro " << fileName << endl;
    }
    else
    {
        for (auto it = points.begin(); it != points.end(); ++it)
        {
            file << "v " << it->getX() << " " << it->getY() << " " << it->getZ() << endl;
        }

        for (auto it = triangles.begin(); it != triangles.end(); ++it)
        {
            file << "f " << it->getX() << " " << it->getY() << " " << it->getZ() << endl;
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
vector<Point> reader(string fileName, vector<Triangle>* triangles){
    ifstream file("../" + fileName);
    vector<Point> points {};
    vector<Point> normals {};
    vector<int> normal_indexes {};

    if (!file)
    {
        cout << "Não é possível abrir o ficheiro " << fileName << endl;
    }
    else
    {
        string line = "v 1 2 3";
        std::regex regex(R"(^(v|f|vn) (-?\d+\.?\d*).*?(?:\/(\d+))? (-?\d+\.?\d*).*?(?:\/(\d+))? (-?\d+\.?\d*).*?(?:\/(\d+))?$)");
        std::smatch match;

        while (std::getline(file, line, '\n')) {
            // [vf] \d+ \d+ \d+
            if (std::regex_search(line, match, regex)){
                if (match[1].str() == "v"){ // Vertice
                    // FORMAT: v x y z

                    Point p(stof(match[2].str()), stof(match[4].str()), stof(match[6].str()));
                    points.push_back(p);
                }
                else if (match[1].str() == "f"){
                    // FORMAT: f pointIndex1(/normalIndex1) pointIndex2(/normalIndex2) pointIndex1(/normalIndex2)

                    // Get the point indexes
                    int pointInd1 = stoi(match[2].str());
                    int pointInd2 = stoi(match[4].str());
                    int pointInd3 = stoi(match[6].str());

                    // Get the points
                    Point p1 = points[pointInd1];
                    Point p2 = points[pointInd2];
                    Point p3 = points[pointInd3];

                    int normalIndex = stoi(match[3].str()); // Assume all normal indexes are the same
                    Point normalVector = normals[normalIndex];

                    // From the 3 points, create 2 vectors
                    Point v1 (p1.getX()-p2.getX(), p1.getY()-p2.getY(), p1.getZ()-p2.getZ());
                    Point v2 (p3.getX()-p2.getX(), p3.getY()-p2.getY(), p3.getZ()-p2.getZ());

                    // Calculate a vector perpendicular to those
                    float cross_P[3];
                    crossProduct(v1.getX(), v1.getY(), v1.getZ(), v2.getX(), v2.getY(), v2.getZ(), cross_P);

                    Point perpendicular (cross_P[0], cross_P[1], cross_P[2]);

                    float norma = sqrt(pow(cross_P[0], 2) + pow(cross_P[1], 2) + pow(cross_P[2], 2)); // Normalization
                    perpendicular.setX(perpendicular.getX()/norma);
                    perpendicular.setY(perpendicular.getY()/norma);
                    perpendicular.setZ(perpendicular.getZ()/norma);

                    // Iguais
                    if (abs(perpendicular.getX() - normalVector.getX()) < 0.1 && abs(perpendicular.getY() - normalVector.getY()) < 0.1 && abs(perpendicular.getZ() -normalVector.getZ()) < 0.1) {
                        Triangle t(pointInd1, pointInd2, pointInd3);
                    } // Simétricos
                    else if (abs(perpendicular.getX() + normalVector.getX()) < 0.1 && abs(perpendicular.getY() + normalVector.getY()) < 0.1 && abs(perpendicular.getZ() + normalVector.getZ()) < 0.1) {
                        Triangle t(pointInd1, pointInd3, pointInd2);
                    }
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