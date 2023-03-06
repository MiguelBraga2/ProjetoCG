#include <stdio.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
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
vector<Triangle> generatePlane(float length, int grid, Point direction, Point initial, bool clockWiseDir, map<string, int> *indexes, int *index){
    int numSquares = grid*grid; // each smaller square has 2 triangles
    float smallerSide = length / grid; // side of each of the smaller squares
    
    vector<Triangle> triangles{}; 
    
    Point base (initial.getX(), initial.getY(), initial.getZ());

    if ((*indexes).count(base.toString()) == 0) {
        (*indexes)[base.toString()] = (*index);
        (*index)++;
    }


    for(int i=0; i<numSquares; i++) {
        // Generate the 4 points for the 2 triangles
        Point p1(base.getX(), base.getY(), base.getZ());
        Point p4(base.getX()+smallerSide*direction.getX(), base.getY()+smallerSide*direction.getY(), base.getZ()+smallerSide*direction.getZ()); // Point in the opposite side of the smaller square
        Point *p2, *p3;

        if ((*indexes).count(p1.toString()) == 0) {
            (*indexes)[p1.toString()] = (*index);
            (*index)++;
        }
        if ((*indexes).count(p4.toString()) == 0) {
            (*indexes)[p4.toString()] = (*index);
            (*index)++;
        }
        

        // Generate the other points of the triangles, depending on the direction of the plane
        if (direction.getX() == 0){
            p2 = new Point (base.getX(), base.getY()+smallerSide*direction.getY(), base.getZ());
            p3 = new Point (base.getX(), base.getY(), base.getZ()+smallerSide*direction.getZ());
        }
        else if (direction.getY() == 0){
            p2 = new Point (base.getX()+smallerSide*direction.getX(), base.getY(), base.getZ());
            p3 = new Point (base.getX(), base.getY(), base.getZ()+smallerSide*direction.getZ());
        }
        else if (direction.getZ() == 0){
            p2 = new Point (base.getX(), base.getY()+smallerSide*direction.getY(), base.getZ());
            p3 = new Point (base.getX()+smallerSide*direction.getX(), base.getY(), base.getZ());
        }

        if ((*indexes).count((*p2).toString()) == 0) {
            (*indexes)[(*p2).toString()] = (*index);
            (*index)++;
        }
        if ((*indexes).count((*p3).toString()) == 0) {
            (*indexes)[(*p3).toString()] = (*index);
            (*index)++;
        }

        // Generate the triangles
        Triangle* t1, *t2;
        if (clockWiseDir == false){
            t1 = new Triangle((*indexes)[p1.toString()], (*indexes)[p4.toString()], (*indexes)[(*p2).toString()]);
            t2 = new Triangle((*indexes)[p1.toString()], (*indexes)[(*p3).toString()], (*indexes)[p4.toString()]);
        }
        else {
            t1 = new Triangle((*indexes)[p1.toString()], (*indexes)[(*p2).toString()], (*indexes)[p4.toString()]);
            t2 = new Triangle((*indexes)[p1.toString()], (*indexes)[p4.toString()], (*indexes)[(*p3).toString()]);
        }

        // Add the triangles to the array
        triangles.push_back(*t1);
        triangles.push_back(*t2);

        // Move the base point
        if (direction.getX() == 0){
            base.setY(base.getY()+smallerSide*direction.getY());
            if (base.getY() == initial.getY() + length*direction.getY()){
                base.setY(initial.getY()); // Back to the begin
                base.setZ(base.getZ() + smallerSide*direction.getZ());
            }
        }
        else if (direction.getY() == 0){
            base.setX(base.getX()+smallerSide*direction.getX());
            if (base.getX() == initial.getX() + length*direction.getX()){
                base.setX(initial.getX()); // Back to the begin
                base.setZ(base.getZ() + smallerSide*direction.getZ());
            }
        }
        else if (direction.getZ() == 0){
            base.setY(base.getY()+smallerSide*direction.getY());
            if (base.getY() == initial.getY() + length*direction.getY()){
                base.setY(initial.getY()); // Back to the begin
                base.setX(base.getX() + smallerSide*direction.getX());
            }
        }
    }

    return triangles;
}


/**
 * Generates a group of triangles, making a cylinder of given radius and height
 * @param radius the radius of the base and top circles of the cylinder
 * @param height the height of the cylinder
 * @param slices vertical divisions of the sphere
 * @return a list of generated triangles
 */
vector<Triangle> drawCylinder(float radius, float height, int slices, map<string, int>* indexes, int* index) {

    int aux = height / 2;
    double delta = (2 * M_PI) / slices;
    double alpha = 0;
    vector<Triangle> triangles{};

    for (int i = 0; i < slices; i++) {
        alpha = i * delta;
        float x1 = radius * sin(alpha);
        float x2 = radius * sin(alpha + delta);
        float z1 = radius * cos(alpha);
        float z2 = radius * cos(alpha + delta);

        Point p1(0, aux, 0);
        Point p2(x1, aux, z1);
        Point p3(x2, aux, z2);
        if ((*indexes).count(p1.toString()) == 0) {
            (*indexes)[p1.toString()] = (*index);
            (*index)++;
        }
        if ((*indexes).count(p2.toString()) == 0) {
            (*indexes)[p2.toString()] = (*index);
            (*index)++;
        }
        if ((*indexes).count(p3.toString()) == 0) {
            (*indexes)[p3.toString()] = (*index);
            (*index)++;
        }
        Triangle t1((*indexes)[p1.toString()], (*indexes)[p2.toString()], (*indexes)[p3.toString()]);
        triangles.push_back(t1);

        Point p4(0, -aux, 0);
        Point p5(x2, -aux, z2);
        Point p6(x1, -aux, z1);
        if ((*indexes).count(p4.toString()) == 0) {
            (*indexes)[p4.toString()] = (*index);
            (*index)++;
        }
        if ((*indexes).count(p5.toString()) == 0) {
            (*indexes)[p5.toString()] = (*index);
            (*index)++;
        }
        if ((*indexes).count(p6.toString()) == 0) {
            (*indexes)[p6.toString()] = (*index);
            (*index)++;
        }
        Triangle t2((*indexes)[p4.toString()], (*indexes)[p5.toString()], (*indexes)[p6.toString()]);
        triangles.push_back(t2);

        Triangle t3((*indexes)[p2.toString()], (*indexes)[p6.toString()], (*indexes)[p5.toString()]);
        triangles.push_back(t3);

        Triangle t4((*indexes)[p2.toString()], (*indexes)[p5.toString()], (*indexes)[p3.toString()]);
        triangles.push_back(t4);
    }

    return triangles;

}

/**
 * Generates a group of triangles, making a cone of given radius and height
 * @param radius the radius of the base circle of the cone
 * @param height the height of the cone
 * @param slices vertical divisions of the cone
 * @param stacks vertical divisions of the cone
 * @return a list of generated triangles
 */
vector<Triangle> drawCone(float radius, float height, int slices, int stacks, map<string, int> *indexes, int *index) {
    vector<Triangle> figure{};
    double alphaDelta = (2 * M_PI) / slices;
    double stackHeight = height / stacks;
    double radiusDec = radius / stacks;

    for (int i = 0; i < slices; i++) {
        double alpha = i * alphaDelta;
        float x1 = radius * sin(alpha);
        float x2 = radius * sin(alpha + alphaDelta);
        float z1 = radius * cos(alpha);
        float z2 = radius * cos(alpha + alphaDelta);

        Point p1(0, 0, 0);
        Point p2(x2, 0, z2);
        Point p3(x1, 0, z1);
        if ((*indexes).count(p1.toString()) == 0) {
            (*indexes)[p1.toString()] = (*index);
            (*index)++;
        }
        if ((*indexes).count(p2.toString()) == 0) {
            (*indexes)[p2.toString()] = (*index);
            (*index)++;
        }
        if ((*indexes).count(p3.toString()) == 0) {
            (*indexes)[p3.toString()] = (*index);
            (*index)++;
        }

        Triangle t1((*indexes)[p1.toString()], (*indexes)[p2.toString()], (*indexes)[p3.toString()]);
        figure.push_back(t1);

        for (int j = 0; j < stacks; j++) {
            double current_radius = radius - j * radiusDec;
            double next_radius = radius - (j + 1) *radiusDec;

            float x3 = current_radius * sin(alpha);
            float z3 = current_radius * cos(alpha);
            float x4 = current_radius * sin(alpha + alphaDelta);
            float z4 = current_radius * cos(alpha + alphaDelta);
            float x5 = next_radius * sin(alpha);
            float z5 = next_radius * cos(alpha);
            float x6 = next_radius * sin(alpha + alphaDelta);
            float z6 = next_radius * cos(alpha + alphaDelta);

            Point p4(x3, stackHeight * j, z3);
            Point p5(x4, stackHeight * j, z4);
            Point p6(x5, stackHeight * (j + 1), z5);
            Point p7(x6, stackHeight * (j + 1), z6);

            if ((*indexes).count(p4.toString()) == 0) {
                (*indexes)[p4.toString()] = (*index);
                (*index)++;
            }
            if ((*indexes).count(p5.toString()) == 0) {
                (*indexes)[p5.toString()] = (*index);
                (*index)++;
            }
            if ((*indexes).count(p6.toString()) == 0) {
                (*indexes)[p6.toString()] = (*index);
                (*index)++;
            }
            if ((*indexes).count(p7.toString()) == 0) {
                (*indexes)[p7.toString()] = (*index);
                (*index)++;
            }


            Triangle t2((*indexes)[p4.toString()], (*indexes)[p5.toString()], (*indexes)[p6.toString()]);
            figure.push_back(t2);
            if (j != stacks - 1) {
                Triangle t3((*indexes)[p5.toString()], (*indexes)[p7.toString()], (*indexes)[p6.toString()]);
                figure.push_back(t3);
            }
        }
    }

    return figure;
}

/**
 * Generates a group of triangles that combined approximate a sphere with a given radius using slices and stacks
 * The sphere is centered in the origin (0,0,0)
 * @param radius the radius of the sphere
 * @param numSlices vertical divisions of the sphere
 * @param numStacks horizontal divisions of the sphere
 * @return
 */
vector<Triangle> drawSphere(float radius, int numSlices, int numStacks, map<string, int>* indexes, int* index){
    float alpha = 2*M_PI/numSlices; // Defines the position around the y axis
    float initialBeta = M_PI/numStacks; // Defines the height
    float beta = -M_PI/2 + (numStacks-1)* initialBeta;
    vector<Triangle> figure {};
    int ind=0;

    for (int i=0; i<numSlices; i++){
        float nextAlpha = (i+1)*alpha;
        Point p1(0,radius, 0);
        Point p2(radius*cos(beta)*sin(i*alpha), radius*sin(beta), radius*cos(i*alpha)*cos(beta));
        Point p3(radius*cos(beta)*sin(nextAlpha), radius*sin(beta), radius*cos(nextAlpha)*cos(beta));

        if ((*indexes).count(p1.toString()) == 0) {
            (*indexes)[p1.toString()] = (*index);
            (*index)++;
        }
        if ((*indexes).count(p2.toString()) == 0) {
            (*indexes)[p2.toString()] = (*index);
            (*index)++;
        }
        if ((*indexes).count(p3.toString()) == 0) {
            (*indexes)[p3.toString()] = (*index);
            (*index)++;
        }

        Triangle t((*indexes)[p1.toString()], (*indexes)[p2.toString()], (*indexes)[p3.toString()]);
        figure.push_back(t);
        ind++;
    }

    for(int i=0; i<numStacks-2; i++){ // Draw each of the vertical divisions
        float currentBeta = -M_PI/2+(i+1)*initialBeta; // height
        float nextBeta = -M_PI/2+(i+2)*initialBeta; // height above


        for (int j=0; j<numSlices; j++){
            float currentAlpha = j*alpha;
            float nextAlpha = (j+1)*alpha;

            Point p1(radius*cos(nextBeta)*sin(currentAlpha), radius*sin(nextBeta), radius*cos(nextBeta)*cos(currentAlpha));
            Point p2(radius*cos(currentBeta)*sin(currentAlpha), radius*sin(currentBeta), radius*cos(currentBeta)*cos(currentAlpha));
            Point p3(radius*cos(currentBeta)*sin(nextAlpha), radius*sin(currentBeta), radius*cos(currentBeta)*cos(nextAlpha));

            if ((*indexes).count(p1.toString()) == 0) {
                (*indexes)[p1.toString()] = (*index);
                (*index)++;
            }
            if ((*indexes).count(p2.toString()) == 0) {
                (*indexes)[p2.toString()] = (*index);
                (*index)++;
            }
            if ((*indexes).count(p3.toString()) == 0) {
                (*indexes)[p3.toString()] = (*index);
                (*index)++;
            }

            Triangle t((*indexes)[p1.toString()], (*indexes)[p2.toString()], (*indexes)[p3.toString()]);
            figure.push_back(t);

            Point p4(radius*cos(nextBeta)*sin(nextAlpha), radius*sin(nextBeta), radius*cos(nextBeta)*cos(nextAlpha));
            Point p5(radius*cos(nextBeta)*sin(currentAlpha), radius*sin(nextBeta), radius*cos(nextBeta)*cos(currentAlpha));
            Point p6(radius*cos(currentBeta)*sin(nextAlpha), radius*sin(currentBeta), radius*cos(currentBeta)*cos(nextAlpha));


            if ((*indexes).count(p4.toString()) == 0) {
                (*indexes)[p4.toString()] = (*index);
                (*index)++;
            }
            if ((*indexes).count(p5.toString()) == 0) {
                (*indexes)[p5.toString()] = (*index);
                (*index)++;
            }
            if ((*indexes).count(p6.toString()) == 0) {
                (*indexes)[p6.toString()] = (*index);
                (*index)++;
            }

            Triangle t2((*indexes)[p4.toString()], (*indexes)[p5.toString()], (*indexes)[p6.toString()]);

            figure.push_back(t2);
        }
    }

    beta = -M_PI/2+initialBeta;

    for (int i=0; i<numSlices; i++){
        float nextAlpha = (i+1)*alpha;

        Point p1(0,-radius, 0);
        Point p2(radius*cos(beta)*sin(nextAlpha), radius*sin(beta), radius*cos(nextAlpha)*cos(beta));
        Point p3(radius*cos(beta)*sin(i*alpha), radius*sin(beta), radius*cos(i*alpha)*cos(beta));

        if ((*indexes).count(p1.toString()) == 0) {
            (*indexes)[p1.toString()] = (*index);
            (*index)++;
        }
        if ((*indexes).count(p2.toString()) == 0) {
            (*indexes)[p2.toString()] = (*index);
            (*index)++;
        }
        if ((*indexes).count(p3.toString()) == 0) {
            (*indexes)[p3.toString()] = (*index);
            (*index)++;
        }
        Triangle t((*indexes)[p1.toString()], (*indexes)[p2.toString()], (*indexes)[p3.toString()]);

        figure.push_back(t);
    }

    return figure;
}

int main(int argc, char** argv)
{
    if (strcmp(argv[1], "sphere") == 0)
    {
        if (argc == 6) 
        {
            int radius = atoi(argv[2]);
            int slices = atoi(argv[3]);
            int stacks = atoi(argv[4]);

            map<string, int> indexes;
            int index = 0;
            vector<Triangle> list = drawSphere(radius, slices, stacks, &indexes, &index);
            writer(argv[5], indexes, list);
        }
        else 
        {
            cout << "Número de argumentos inválido";
        }
    }
    else if (strcmp(argv[1], "cone") == 0) 
    {
        if (argc == 7)
        {
            
            map<string, int> indexes;
            int index = 0;
            
            vector<Triangle> triangles = drawCone(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stof(argv[5]), &indexes, &index);
            writer(argv[6], indexes, triangles);
        
        }
        else 
        {
            cout << "Número de argumentos inválido";
        }   
    }
    else if (strcmp(argv[1], "box") == 0)
    {
        if (argc == 5)
        {
            float side = atof(argv[2]);
            int grid = atoi(argv[3]);

            map<string, int> indexes;
            int index = 0;

            vector<Triangle> triangles = generatePlane(side, grid, Point(1, 0, 1), Point(-side / 2, -side / 2, -side / 2), true, &indexes, &index);
            vector<Triangle> aux = generatePlane(side, grid, Point(1, 0, 1), Point(-side / 2, side / 2, -side / 2), false, &indexes, &index);
            triangles.insert(triangles.end(), aux.begin(), aux.end());
            aux = generatePlane(side, grid, Point(0, -1, -1), Point(side / 2, side / 2, side / 2), true, &indexes, &index);
            triangles.insert(triangles.end(), aux.begin(), aux.end());
            aux = generatePlane(side, grid, Point(0, -1, -1), Point(-side / 2, side / 2, side / 2), false, &indexes, &index);
            triangles.insert(triangles.end(), aux.begin(), aux.end());
            aux = generatePlane(side, grid, Point(1, -1, 0), Point(-side / 2, side / 2, side / 2), true, &indexes, &index);
            triangles.insert(triangles.end(), aux.begin(), aux.end());
            aux = generatePlane(side, grid, Point(1, -1, 0), Point(-side / 2, side / 2, -side / 2), false, &indexes, &index);
            triangles.insert(triangles.end(), aux.begin(), aux.end());

            writer(argv[4], indexes, triangles);
        }
        else 
        {
            cout << "Número de argumentos inválido";
        }       

    }
    else if (strcmp(argv[1], "plane") == 0) {
        if (argc == 5) {
            
            float length = stof(argv[2]);
            map<string, int> indexes;
            int index = 0;

            vector<Triangle> triangles = generatePlane(length, stoi(argv[3]), Point(1, 0, 1), Point(-length / 2, 0, -length / 2), false, &indexes, &index);
            writer(argv[4], indexes, triangles);

        }
        else
        {
            cout << "Figura desconhecida";
        }
    }
    else if (strcmp(argv[1], "cylinder") == 0) {
        if (argc == 6) {

            map<string, int> indexes;
            int index = 0;

            vector<Triangle> triangles = drawCylinder(stof(argv[2]), stof(argv[3]), stof(argv[4]), &indexes, &index);
            writer(argv[5], indexes, triangles);

        }
        else 
        {
            cout << "Figura desconhecida";
        }

    }
}