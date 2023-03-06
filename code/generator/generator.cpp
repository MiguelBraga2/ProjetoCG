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

    float aux = height / 2;
    double delta = (2 * M_PI) / slices;
    double alpha;
    float x1, x2, z1, z2;
    vector<Triangle> triangles{};

    for (int i = 0; i < slices; i++) {
        alpha = i * delta;
        x1 = radius * sin(alpha);
        x2 = radius * sin(alpha + delta);
        z1 = radius * cos(alpha);
        z2 = radius * cos(alpha + delta);

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
    float x1, z1, x2, z2, x3, z3, x4, z4, x5, z5, x6, z6;
    double current_radius, next_radius, alpha;

    for (int i = 0; i < slices; i++) {
        alpha = i * alphaDelta;
        x1 = radius * sin(alpha);
        x2 = radius * sin(alpha + alphaDelta);
        z1 = radius * cos(alpha);
        z2 = radius * cos(alpha + alphaDelta);

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
            current_radius = radius - j * radiusDec;
            next_radius = radius - (j + 1) *radiusDec;

            x3 = current_radius * sin(alpha);
            z3 = current_radius * cos(alpha);
            x4 = current_radius * sin(alpha + alphaDelta);
            z4 = current_radius * cos(alpha + alphaDelta);
            x5 = next_radius * sin(alpha);
            z5 = next_radius * cos(alpha);
            x6 = next_radius * sin(alpha + alphaDelta);
            z6 = next_radius * cos(alpha + alphaDelta);

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
    vector<Triangle> figure{};
    float alphaDelta = (2 * M_PI) / numSlices; // Defines the position around the y axis
    float alpha, nextAlpha;
    float betaDelta = M_PI / numStacks; // Defines the height
    float beta, nextBeta;
    

    for (int i = 0; i < numSlices; i++) {
        alpha = i * alphaDelta;
        nextAlpha = (i + 1) * alphaDelta;
        beta = - M_PI / 2 + betaDelta;

        Point p1(0, -radius, 0);
        Point p2(radius * cos(beta) * sin(nextAlpha), radius * sin(beta), radius * cos(nextAlpha) * cos(beta));
        Point p3(radius * cos(beta) * sin(alpha), radius * sin(beta), radius * cos(alpha) * cos(beta));

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

        for (int j = 1; j < numStacks - 1; j++) {
            beta = j * betaDelta - M_PI / 2;
            nextBeta = (j + 1) * betaDelta - M_PI / 2; // height above

            Point p4(radius * cos(nextBeta) * sin(alpha), radius * sin(nextBeta), radius * cos(nextBeta) * cos(alpha));
            Point p5(radius * cos(beta) * sin(alpha), radius * sin(beta), radius * cos(beta) * cos(alpha));
            Point p6(radius * cos(beta) * sin(nextAlpha), radius * sin(beta), radius * cos(beta) * cos(nextAlpha));

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

            Triangle t1((*indexes)[p4.toString()], (*indexes)[p5.toString()], (*indexes)[p6.toString()]);
            figure.push_back(t1);

            Point p7(radius * cos(nextBeta) * sin(nextAlpha), radius * sin(nextBeta), radius * cos(nextBeta) * cos(nextAlpha));
            Point p8(radius * cos(nextBeta) * sin(alpha), radius * sin(nextBeta), radius * cos(nextBeta) * cos(alpha));
            Point p9(radius * cos(beta) * sin(nextAlpha), radius * sin(beta), radius * cos(beta) * cos(nextAlpha));

            if ((*indexes).count(p7.toString()) == 0) {
                (*indexes)[p7.toString()] = (*index);
                (*index)++;
            }
            if ((*indexes).count(p8.toString()) == 0) {
                (*indexes)[p8.toString()] = (*index);
                (*index)++;
            }
            if ((*indexes).count(p9.toString()) == 0) {
                (*indexes)[p9.toString()] = (*index);
                (*index)++;
            }

            Triangle t2((*indexes)[p7.toString()], (*indexes)[p8.toString()], (*indexes)[p9.toString()]);
            figure.push_back(t2);
        }
        
        beta = M_PI / 2 - betaDelta;
        Point p4(0, radius, 0);
        Point p5(radius * cos(beta) * sin(alpha), radius * sin(beta), radius * cos(beta) * cos(alpha));
        Point p6(radius * cos(beta) * sin(nextAlpha), radius * sin(beta), radius * cos(beta) * cos(nextAlpha));

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
        Triangle t1((*indexes)[p4.toString()], (*indexes)[p5.toString()], (*indexes)[p6.toString()]);
        figure.push_back(t1);
    }

    return figure;
}

vector<Triangle> drawTorus(float innerRadius, float outerRadius, float slices, float stacks, map<string, int>* indexes, int* index){
    float alpha = 2*M_PI/slices; // Defines the position around the y axis
    float initialBeta = M_PI/stacks; // Defines the height
    vector<Triangle> figure {};

    for(int i=0; i<slices; i++){ // Percorrer as slices
        float beta = -M_PI/2;
        float currentAlpha = i*alpha;
        for(int j=0; j<2*stacks; j++){ // Percorrer as stacks
            float nextBeta = beta+initialBeta;
            float nextAlpha = currentAlpha+alpha;
        }
    }
}


int main(int argc, char** argv)
{
    if (strcmp(argv[1], "sphere") == 0)
    {
        if (argc == 6) 
        {

            map<string, int> indexes;
            int index = 0;

            vector<Triangle> list = drawSphere(stof(argv[2]), stoi(argv[3]), stoi(argv[4]), &indexes, &index);
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
            float side = stof(argv[2]);
            int grid = stoi(argv[3]);

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
    else if (strcmp(argv[1], "torus") == 0){
        if (argc == 7) {
            // Torus InnerRadius OuterRadius Slices Stacks

            map<string, int> indexes;
            int index=0;

            vector<Triangle> triangles = drawTorus(stof(argv[2]), stof(argv[3]), stof(argv[4]), stof(argv[5]), &indexes, &index);
            writer(argv[6], indexes, triangles);
        }
        else
        {
            cout << "Figura desconhecida";
        }
    }
}