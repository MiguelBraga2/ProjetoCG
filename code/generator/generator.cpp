#include <stdio.h>
#include <string.h>

#include <iostream>
#include "point.hpp"
#include "triangle.hpp"

using namespace std;

Triangle* generatePlane(float length, int grid){
    int numTriangles = grid*grid*2;
    Triangle* figure = (Triangle*) malloc(numTriangles*sizeof(Triangle));
    float x, z;
    printf("Length: %dGrid: %d\n", length, grid);
    x = -length/2;
    z = -length/2;

    // 18
    for (int i=0; i<numTriangles/2; i++){
        Point p1 {x, 0, z};
        Point p2 {x+length/3, 0, z};
        Point p3 {x, 0, z+length/3};
        Point p4 {x+length/3, 0, z+length/3};

        Triangle t1 {p1, p3, p4};
        Triangle t2 {p1, p2, p4};

        figure[i*2] = t1;
        figure[i*2+1] = t2;

        // Movimentar o ponto inicial do triângulo
        if (x == 2*length/3){
            x = -length/2;
        }
        else {
            x += length/3;
        }
        if (z == 2*length/3){
            z = -length/2;
        }
        else {
            z += length/3;
        }
    }

    return figure;
}

int main(int argc, char** argv)
{
    if (strcmp(argv[1], "sphere") == 0)
    {
        if (argc == 6){
            int radius = atoi(argv[2]);
            int slices = atoi(argv[3]);
            int stacks = atoi(argv[4]);
            char* file = argv[5];

            std::cout << "Raio: " << radius << endl;
            cout << "Slices: " << slices << endl;
            cout << "Stacks: " << stacks << endl;
            cout << "File: " << file << endl;
        }
        else {
            cout << "Número de argumentos inválido";
        }        
    }
    else if (strcmp(argv[1], "cone") == 0) 
    {
        if (argc == 7){
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
        }        
    }
    else if (strcmp(argv[1], "box") == 0)
    {
        if (argc == 5){
            int side = atoi(argv[2]);
            int grid = atoi(argv[3]); 
            char* file = argv[4];

            cout << "Lado: " << side << endl;
            cout << "Grid: " << grid << endl;
            cout << "File: " << file << endl;
        }
        else {
            cout << "Número de argumentos inválido";
        }        

        // length: 2
        // d = grid (each side is divided in a grid): 3
        // file: 4
        // numero de vertices -> 6 lados -> d*d *2 vertices por lado

        cout << "Cubo";

    }
    else if (strcmp(argv[1], "plane") == 0)
    {
        // length: 2
        // d = number of divisions along each axis: 3
        // file: 4
        // numero de vertices -> d*d quadraros *2 triangulos

        if (argc == 5){
            int side = atoi(argv[2]);
            int grid = atoi(argv[3]); 
            char* file_path = argv[4];

            cout << "Lado: " << side << endl;
            cout << "Grid: " << grid << endl;
            cout << "File: " << file_path << endl;

            FILE* file = fopen(file_path, "w");
            
            Triangle* list = generatePlane(side, grid);

            for(int i=0; i<side*side*2; i++){
                printTriangle(list[i]);
            }

        }
        else {
            cout << "Número de argumentos inválido";
        }        

        cout << "Plano";

    }
    else {
        cout << "Figura desconhecida";
    }


}