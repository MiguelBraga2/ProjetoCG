#include <stdio.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../shared/point.hpp"
#include "../shared/triangle.hpp"
#include "../shared/IO.hpp"

using namespace std;


/**
 * Generates a group of triangles that combine into a grid, making a square (plane) of given length
 * @param length the length of the larger square
 * @param grid number of smaller squares per side
 * @param direction defines the direction of the plane (for each coordinate, 0 - no direction)
 * For example, 0 in the y direction means the plane is parallel to the y=0 plane
 * @param initial the plane to start generating the plane
 * @param clockWiseDir true if direction is set to clockwise, false if direction is set to counterclockwise
 * @return a list of generated triangles
 */
vector<float> generatePlane(float length, int grid, Point direction, Point initial, bool clockWiseDir, vector<unsigned int> *indexes, int *index){
    vector<float> vertices;
    float step = length / grid; // side of each of the smaller squares
     
    Point base (initial.getX(), initial.getY(), initial.getZ());

    for(int i=0; i<grid; i++) {
        
        vertices.push_back(base.getX());
        vertices.push_back(base.getY());
        vertices.push_back(base.getZ());

        if (direction.getX() == 0) {
            vertices.push_back(base.getX());
            vertices.push_back(base.getY());
            vertices.push_back(base.getZ() + step * direction.getZ());
        }
        else if (direction.getY() == 0) {
            vertices.push_back(base.getX() + step * direction.getX());
            vertices.push_back(base.getY());
            vertices.push_back(base.getZ());
        }
        else if (direction.getZ() == 0) {
            vertices.push_back(base.getX() + step * direction.getX());
            vertices.push_back(base.getY());
            vertices.push_back(base.getZ());
        }

        for (int j = 0; j < grid; j++) {

            if (direction.getX() == 0) {
                vertices.push_back(base.getX());
                vertices.push_back(base.getY() + step * direction.getY());
                vertices.push_back(base.getZ());
            }
            else if (direction.getY() == 0) {
                vertices.push_back(base.getX());
                vertices.push_back(base.getY());
                vertices.push_back(base.getZ() + step * direction.getZ());
            }
            else if (direction.getZ() == 0) {
                vertices.push_back(base.getX());
                vertices.push_back(base.getY() + step * direction.getY());
                vertices.push_back(base.getZ());
            }

            vertices.push_back(base.getX() + step * direction.getX());
            vertices.push_back(base.getY() + step * direction.getY());
            vertices.push_back(base.getZ() + step * direction.getZ());          
            
            if (clockWiseDir == false) {
                indexes->push_back(*index);
                indexes->push_back((*index) + 3);
                indexes->push_back((*index) + 1);

                indexes->push_back(*index);
                indexes->push_back((*index) + 2);
                indexes->push_back((*index) + 3);
            } else {
                indexes->push_back(*index);
                indexes->push_back((*index) + 1);
                indexes->push_back((*index) + 3);

                indexes->push_back(*index);
                indexes->push_back((*index) + 3);
                indexes->push_back((*index) + 2);
            }

            // Move the base point
            if (direction.getX() == 0) {
                base.setY(base.getY() + step * direction.getY()); 
            } else if (direction.getY() == 0) {
                base.setZ(base.getZ() + step * direction.getZ());
            } else if (direction.getZ() == 0) {
                base.setY(base.getY() + step * direction.getY());
            }

            (*index) += 2;
        }
        if (direction.getX() == 0) {
            base.setZ(base.getZ() + step * direction.getZ());
            base.setY(initial.getY());
        } else if (direction.getY() == 0) {
            base.setX(base.getX() + step * direction.getX());
            base.setZ(initial.getZ());
        } else if (direction.getZ() == 0) {
            base.setX(base.getX() + step * direction.getX());
            base.setY(initial.getY());
        }

        (*index) += 2;
    }

    return vertices;
}


/**
 * Generates a group of triangles, making a cylinder of given radius and height
 * @param radius the radius of the base and top circles of the cylinder
 * @param height the height of the cylinder
 * @param slices vertical divisions of the sphere
 * @return a list of generated triangles
 */
vector<float> generateCylinder(float radius, float height, int slices,vector<unsigned int>* indexes) {
    vector<float> vertices;
    float aux = height / 2;
    double sliceStep = (2 * M_PI) / slices;
    int index = 0;
 
    for (int i = 0; i < slices; i++) {
        vertices.push_back(0);
        vertices.push_back(-aux);
        vertices.push_back(0);

        vertices.push_back(radius * sin(i * sliceStep));
        vertices.push_back(-aux);
        vertices.push_back(radius * cos(i * sliceStep));

        vertices.push_back(radius * sin((i + 1) * sliceStep));
        vertices.push_back(-aux);
        vertices.push_back(radius * cos((i + 1) * sliceStep));

        vertices.push_back(radius * sin(i * sliceStep));
        vertices.push_back(aux);
        vertices.push_back(radius * cos(i * sliceStep));

        vertices.push_back(radius * sin((i + 1) * sliceStep));
        vertices.push_back(aux);
        vertices.push_back(radius * cos((i + 1) * sliceStep));

        vertices.push_back(0);
        vertices.push_back(aux);
        vertices.push_back(0);

        indexes->push_back(index);
        indexes->push_back(index + 2);
        indexes->push_back(index + 1);

        indexes->push_back(index + 1);
        indexes->push_back(index + 2);
        indexes->push_back(index + 3);

        indexes->push_back(index + 2);
        indexes->push_back(index + 4);
        indexes->push_back(index + 3);

        indexes->push_back(index + 3);
        indexes->push_back(index + 4);
        indexes->push_back(index + 5);

        index += 6;
    }

    return vertices;

}

/**
 * Generates a group of triangles, making a cone of given radius and height
 * @param radius the radius of the base circle of the cone
 * @param height the height of the cone
 * @param slices vertical divisions of the cone
 * @param stacks vertical divisions of the cone
 * @return a list of generated triangles
 */
vector<float> generateCone(float radius, float height, int slices, int stacks, vector<unsigned int>* indexes) {
    vector<float> vertices;
    double sliceStep = 2 * M_PI / slices;
    double stackStep = height / stacks;
    double radiusDec = radius / stacks;
    int index = 0;

    for (int i = 0; i < slices; i++) {
        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(0);
        
        vertices.push_back(radius * sin(i * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos(i * sliceStep));

        vertices.push_back(radius * sin((i + 1) * sliceStep));
        vertices.push_back(0);
        vertices.push_back(radius * cos((i + 1) * sliceStep));

        indexes->push_back(index);
        indexes->push_back(index + 2);
        indexes->push_back(index + 1);
        
        index++;

        for (int j = 0; j < stacks - 1; j++) {
            vertices.push_back((radius - (j + 1) * radiusDec) * sin(i * sliceStep));
            vertices.push_back((j + 1) * stackStep);
            vertices.push_back((radius - (j + 1) * radiusDec) * cos(i * sliceStep));

            vertices.push_back((radius - (j + 1) * radiusDec) * sin((i + 1) * sliceStep));
            vertices.push_back((j + 1) * stackStep);
            vertices.push_back((radius - (j + 1) * radiusDec) * cos((i + 1) * sliceStep));

            indexes->push_back(index);
            indexes->push_back(index + 1);
            indexes->push_back(index + 2);

            indexes->push_back(index + 1);
            indexes->push_back(index + 3);
            indexes->push_back(index + 2);

            index += 2;
        }

        vertices.push_back(0);
        vertices.push_back(height);
        vertices.push_back(0);

        indexes->push_back(index);
        indexes->push_back(index + 1);
        indexes->push_back(index + 2);

        index += 3;
    }

    return vertices;
}

/**
 * Generates a group of triangles that combined approximate a sphere with a given radius using slices and stacks
 * The sphere is centered in the origin (0,0,0)
 * @param radius the radius of the sphere
 * @param slices vertical divisions of the sphere
 * @param stacks horizontal divisions of the sphere
 * @return
 */
vector<float> generateSphere(float radius, int slices, int stacks, vector<unsigned int> *indexes) {
    vector<float> vertices;
    
    float sliceStep = 2 * M_PI / slices; 
    float stackStep = M_PI / stacks;
    int index = 0;
    
    for (int i = 0; i < slices; i++) {
        vertices.push_back(0);
        vertices.push_back(-radius);
        vertices.push_back(0);

        vertices.push_back(radius * cos(-M_PI / 2 + stackStep) * sin(i * sliceStep));  
        vertices.push_back(radius * sin(-M_PI / 2 + stackStep));
        vertices.push_back(radius * cos(-M_PI / 2 + stackStep) * cos(i * sliceStep));

        vertices.push_back(radius * cos(-M_PI / 2 + stackStep) * sin((i + 1) * sliceStep));
        vertices.push_back(radius * sin(-M_PI / 2 + stackStep));
        vertices.push_back(radius * cos(-M_PI / 2 + stackStep) * cos((i + 1) * sliceStep));
        
        indexes->push_back(index);
        indexes->push_back(index + 2);
        indexes->push_back(index + 1);
        
        index++;

        for (int j = 1; j < stacks - 1; j++) {
            vertices.push_back(radius * cos(-M_PI / 2 + (j + 1) * stackStep) * sin(i * sliceStep));
            vertices.push_back(radius * sin(-M_PI / 2 + (j + 1) * stackStep));
            vertices.push_back(radius * cos(-M_PI / 2 + (j + 1) * stackStep) * cos(i * sliceStep));

            vertices.push_back(radius * cos(-M_PI / 2 + (j + 1) * stackStep) * sin((i + 1) * sliceStep));
            vertices.push_back(radius * sin(-M_PI / 2 + (j + 1) * stackStep));
            vertices.push_back(radius * cos(-M_PI / 2 + (j + 1) * stackStep) * cos((i + 1) * sliceStep));

            indexes->push_back(index);
            indexes->push_back(index + 1);
            indexes->push_back(index + 2);

            indexes->push_back(index + 1);
            indexes->push_back(index + 3);
            indexes->push_back(index + 2);
            
            index += 2;
        }

        vertices.push_back(0);
        vertices.push_back(radius);
        vertices.push_back(0);

        indexes->push_back(index);
        indexes->push_back(index + 1);
        indexes->push_back(index + 2);

        index += 3;
    }

    return vertices;
}


vector<float> drawTorus(float innerRadius, float outerRadius, float slices, float stacks, vector<unsigned int>* indexes){
    vector<float> vertices;
   
    float alpha = 2 * M_PI / slices; // Defines the position around the y axis
    float beta = M_PI / stacks; // Defines the height
    float radius = (outerRadius - innerRadius) / 2;
    float distanceToOrigin = innerRadius + radius;
    int index = 0;

    for(int i=0; i < slices; i++){ // Percorrer as slices

        Point c1(distanceToOrigin * sin(i * alpha), 0, distanceToOrigin * cos(i * alpha));
        Point c2(distanceToOrigin * sin((i + 1) * alpha), 0, distanceToOrigin * cos((i + 1) * alpha));

        vertices.push_back(c1.getX());
        vertices.push_back(-radius);
        vertices.push_back(c1.getZ());

        vertices.push_back(c2.getX());
        vertices.push_back(-radius);
        vertices.push_back(c2.getZ());

        for(int j = 0; j < 2 * stacks; j++){ // Percorrer as stacks

            Point p1 (radius * cos(-M_PI / 2 + (j + 1) * beta) * sin(i * alpha), radius * sin(-M_PI / 2 + (j + 1) * beta), radius * cos(-M_PI / 2 + (j + 1) * beta) * cos(i * alpha));
            Point p2 (radius * cos(-M_PI / 2 + (j + 1) * beta) * sin((i + 1) * alpha), radius * sin(-M_PI / 2 + (j + 1) * beta), radius * cos(-M_PI / 2 + (j + 1) * beta) * cos((i + 1) * alpha));

            vertices.push_back(c1.getX() + p1.getX());
            vertices.push_back(c1.getY() + p1.getY());
            vertices.push_back(c1.getZ() + p1.getZ());

            vertices.push_back(c2.getX() + p2.getX());
            vertices.push_back(c2.getY() + p2.getY());
            vertices.push_back(c2.getZ() + p2.getZ());

            indexes->push_back(index);
            indexes->push_back(index + 1);
            indexes->push_back(index + 2);

            indexes->push_back(index + 1);
            indexes->push_back(index + 3);
            indexes->push_back(index + 2);

            index += 2;
        }

        index += 2;
    }

    return vertices;
}



int main(int argc, char** argv) {
    if (strcmp(argv[1], "sphere") == 0) {
        if (argc == 6) {
            vector<unsigned int> indexes;
            vector<float> vertices = generateSphere(stof(argv[2]), stoi(argv[3]), stoi(argv[4]), &indexes);
            writer(argv[5], indexes, vertices);
        } else {
            cout << "Número de argumentos inválido" << endl;
        }
    }
    else if (strcmp(argv[1], "cone") == 0) {
        if (argc == 7) {
            vector<unsigned int> indexes;
            vector<float> vertices = generateCone(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi(argv[5]), &indexes);
            writer(argv[6], indexes, vertices);
        }
        else {
            cout << "Número de argumentos inválido" << endl;
        }
    }
    else if (strcmp(argv[1], "box") == 0) {
        if (argc == 5) {
            float side = stof(argv[2]);
            int grid = stoi(argv[3]);

            vector<unsigned int> indexes;
            int index = 0;

            vector<float> vertices = generatePlane(side, grid, Point(1, 0, 1), Point(-side / 2, -side / 2, -side / 2), true, &indexes, &index);
            vector<float> aux = generatePlane(side, grid, Point(1, 0, 1), Point(-side / 2, side / 2, -side / 2), false, &indexes, &index);
            vertices.insert(vertices.end(), aux.begin(), aux.end());
            aux = generatePlane(side, grid, Point(0, -1, -1), Point(side / 2, side / 2, side / 2), false, &indexes, &index);
            vertices.insert(vertices.end(), aux.begin(), aux.end());
            aux = generatePlane(side, grid, Point(0, -1, -1), Point(-side / 2, side / 2, side / 2), true, &indexes, &index);
            vertices.insert(vertices.end(), aux.begin(), aux.end());
            aux = generatePlane(side, grid, Point(1, -1, 0), Point(-side / 2, side / 2, side / 2), false, &indexes, &index);
            vertices.insert(vertices.end(), aux.begin(), aux.end());
            aux = generatePlane(side, grid, Point(1, -1, 0), Point(-side / 2, side / 2, -side / 2), true, &indexes, &index);
            vertices.insert(vertices.end(), aux.begin(), aux.end());
            writer(argv[4], indexes, vertices);
        }
        else {
            cout << "Número de argumentos inválido" << endl;
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
            cout << "Número de argumentos inválido" << endl;
        }
    }
    else if (strcmp(argv[1], "cylinder") == 0) {
        if (argc == 6) {
            vector<unsigned int> indexes;
            vector<float> vertices = generateCylinder(stof(argv[2]), stof(argv[3]), stoi(argv[4]), &indexes);
            writer(argv[5], indexes, vertices);
        }
        else {
            cout << "Número de argumentos inválido" << endl;
        }
    }
    else if (strcmp(argv[1], "torus") == 0) {
        if (argc == 7) { // torus InnerRadius OuterRadius Slices Stacks
            vector<unsigned int> indexes;
            vector<float> vertices = drawTorus(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi(argv[5]), &indexes);
            writer(argv[6], indexes, vertices);
        }
        else {
            cout << "Número de argumentos inválido" << endl;
        }
    }
    else {
        cout << "Figura desconhecida" << endl;
    }
}