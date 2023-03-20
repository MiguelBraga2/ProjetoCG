//
// Created by mike on 20-03-2023.
//

#ifndef ENGINE_GROUP_H
#define ENGINE_GROUP_H

#include <vector>
#include "Transformations/Transformation.h"
using namespace std;

class Group {
private:
    vector<Transformation> transformations;
    vector<string> models;
    vector<Group> subgroups;

public:
    void drawGroup();
};


#endif //ENGINE_GROUP_H
