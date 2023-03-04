//
// Created by mike on 03-03-2023.
//

#include "Parser.hpp"
#include <fstream>
#include <regex>
#include <string>

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

void writer(list<Point> points, list<Point> triangles, string fileName){
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

// Devolve os pontos
list<Point> reader(string fileName, list<Point>* triangles, list<Point>* normals, list<int> normal_indexes){
    ifstream file("../" + fileName);
    list<Point> points {};

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
                if (match[1].str() == "v"){
                    // Vertice
                    Point p(stof(match[2].str()), stof(match[4].str()), stof(match[6].str()));
                    points.push_back(p);
                }
                else if (match[1].str() == "f"){
                    Point p(stof(match[2].str()), stof(match[4].str()), stof(match[6].str()));
                    triangles->push_back(p);
                    normal_indexes.push_back(stoi(match[3]));
                }
                else if (match[1].str() == "vn"){
                    Point p(stof(match[2].str()), stof(match[4].str()), stof(match[6].str()));
                    normals->push_back(p);
                }
            }
        }

        for (auto it = points.begin(); it != points.end(); ++it)
        {
            cout << "v " << it->getX() << " " << it->getY() << " " << it->getZ() << endl;
        }
        return points;
    }

}