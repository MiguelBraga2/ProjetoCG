#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <list>
#include "../shared/point.hpp"
#include "../shared/triangle.hpp"

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
list<Triangle> generatePlane(float length, int grid, Point direction, Point initial, bool clockWiseDir){
    int numSquares = grid*grid; // each smaller square has 2 triangles
    float smallerSide = length / grid; // side of each of the smaller squares
    list<Triangle> triangles{}; 
    Point base (initial.getX(), initial.getY(), initial.getZ());

    for(int i=0; i<numSquares; i++) {
        // Generate the 4 points for the 2 triangles
        Point p1(base.getX(), base.getY(), base.getZ());
        Point p4(base.getX()+smallerSide*direction.getX(), base.getY()+smallerSide*direction.getY(), base.getZ()+smallerSide*direction.getZ()); // Point in the opposite side of the smaller square
        Point *p2, *p3;

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

        // Generate the triangles
        Triangle* t1, *t2;
        if (clockWiseDir == false){
            t1 = new Triangle(p1, p4, *p2);
            t2 = new Triangle(p1, *p3, p4);
        }
        else {
            t1 = new Triangle(p1, *p2, p4);
            t2 = new Triangle(p1, p4, *p3);
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

void write_triangles(string fileName, list<Triangle> triangles) 
{
    ofstream file("../" + fileName, ios::out | ios::binary);
    if (!file)
    {
        cout << "Não é possível abrir o ficheiro " << fileName << endl;
    }
    else
    {   
        int size = triangles.size();
        file.write((char*) &size, sizeof(int));

        for (auto it = triangles.begin(); it != triangles.end(); ++it)
        {
            file.write((char*) &(*it), sizeof(Triangle));
        }
    }
}


/**
 * Generates a group of triangles that combined approximate a sphere with a given radius using slices and stacks
 * The sphere is centered in the origin (0,0,0)
 * @param radius the radius of the sphere
 * @param numSlices vertical divisions of the sphere
 * @param numStacks horizontal divisions of the sphere
 * @return
 */
Triangle* generateSphere(float radius, int numSlices, int numStacks){
    Point bottomPoint = Point (0, -radius, 0);
    Point topPoint = Point(0, radius, 0);
    // Generate triangles bottom-up

    // Generate numStacks triangles in the South Pole
    // The vertice of all triangles is the bottom of the sphere

    // Generate (numSlices-2) triangles in the middle

    // Generate numStacks triangles in the North Pole
    // The vertice of all triangles is the top of the sphere
    return new Triangle();
}

int main(int argc, char** argv)
{
    if (strcmp(argv[1], "sphere") == 0)
    {
        if (argc == 6) 
        {
            /*int radius = atoi(argv[2]);
            int slices = atoi(argv[3]);
            int stacks = atoi(argv[4]);
            char* file = argv[5];

            Triangle* list = generateSphere(radius, slices, stacks);

            for(int i=0; i<sizeof(list)/sizeof(Triangle); i++)
            {
                cout << list[i].toString() << endl;
            }*/
        }
        else 
        {
            cout << "Número de argumentos inválido";
        }
    }
    else if (strcmp(argv[1], "cone") == 0) 
    {
        /*if (argc == 7){
            int radius = atoi(argv[2]);
            int height = atoi(argv[3]);
            int slices = atoi(argv[4]);
            int stacks = atoi(argv[5]);
            char* file = argv[5];

            cout << "Raio: " << radius << endl;
            cout << "Altura: " << height << endl;
            cout << "Slices: " << slices << endl;
            cout << "Stacks: " << stacks << endl;
            cout << "File: " << file << endl;
        }
        else {
            cout << "Número de argumentos inválido";
        }*/
    }
    else if (strcmp(argv[1], "box") == 0)
    {
        if (argc == 5)
        {
            float side = atof(argv[2]);
            int grid = atoi(argv[3]); 
            list<Triangle> triangles{};

            triangles.splice(triangles.end(), generatePlane(side, grid, Point(1, 0, 1), Point(-side / 2, -side / 2, -side / 2), true));
            triangles.splice(triangles.end(), generatePlane(side, grid, Point(1, 0, 1), Point(-side / 2, side / 2, -side / 2), false));
            triangles.splice(triangles.end(), generatePlane(side, grid, Point(0, -1, -1), Point(side / 2, side / 2, side / 2), true));
            triangles.splice(triangles.end(), generatePlane(side, grid, Point(0, -1, -1), Point(-side / 2, side / 2, side / 2), false));
            triangles.splice(triangles.end(), generatePlane(side, grid, Point(1, -1, 0), Point(-side / 2, side / 2, side / 2), true));
            triangles.splice(triangles.end(), generatePlane(side, grid, Point(1, -1, 0), Point(-side / 2, side / 2, -side / 2), false));
            write_triangles(argv[4], triangles);
        }
        else 
        {
            cout << "Número de argumentos inválido";
        }       

    }
    else if (strcmp(argv[1], "plane") == 0) {
        if (argc == 5) {
            float length = atof(argv[2]);
            int grid = atoi(argv[3]);

            list<Triangle> triangles = generatePlane(length, grid, Point(1, 0, 1), Point(-length / 2, 0, -length / 2), false);
            write_triangles(argv[4], triangles);

        }
        else
        {
            cout << "Figura desconhecida";
        }
    }
}