#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <IL/il.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <cstdio>
#include <map>
#include <tuple>
#include <iostream>

int startX, startY, tracking = 0;

float alpha = 0, beta = 35, r = 10;

int red = 1, green = 100, blue = 0;

float k = 1;

int indColors = 0;

std::vector<std::tuple<float, float, float>> colorsVec;

int minecraftMode = 0; // 0 - Construir, 1 - Destruir

GLuint buffers[4];

std::map<std::tuple<float, float, float>, unsigned int> mapPointIndex;

std::vector<float> vertices;
std::vector<unsigned int> indexes;
std::vector<float> colors;
std::vector<float> normalColors;

float nextPosX[255][255][255];
float nextPosY[255][255][255];
float nextPosZ[255][255][255];

float currPosX[255][255][255];
float currPosY[255][255][255];
float currPosZ[255][255][255];

int vertexCount = 0, indexCount = 0, numIndex = 0, cubeIndex = 0;

float camX = 1, camY = 0, camZ = 1;
float lx, ly, lz;
float dx, dy, dz;

int timebase = 0, frame = 0;

unsigned char* imageData;

unsigned int t, tw, th;

float getHeight(int i, int j) {
    return imageData[i * tw + j];
}

float getHeightf(float x, float z) {
    float x1 = floor(x);
    float x2 = x1 + 1;
    float z1 = floor(z);
    float z2 = z1 + 1;

    float fz = z - z1;
    float fx = x - x1;

    float h_x1_z = getHeight(x1 + 128, z1 + 128) * (1 - fz) + getHeight(x1 + 128, z2 + 128) * fz;
    float h_x2_z = getHeight(x2 + 128, z1 + 128) * (1 - fz) + getHeight(x2 + 128, z2 + 128) * fz;

    return  h_x1_z * (1 - fx) + h_x2_z * fx;
}

void aux() {
    dx = lx - camX;
    dy = 0;
    dz = lz - camZ;

    camX = camX + k * dx;
    camY = camY + k * dy;
    camZ = camZ + k * dz;

    lx = lx + k * dx;
    ly = ly + k * dy;
    lz = lz + k * dz;
}

void aux1() {
    lx = camX + sin(alpha);
    ly = camY;
    lz = camZ + cos(alpha);
}

void produtoExterno() {
    float rx = -dz;
    float ry = 0;
    float rz = dx - dy;
    camX = camX + k * rx;
    camY = camY + k * ry;
    camZ = camZ + k * rz;

    lx = lx + k * rx;
    ly = ly + k * ry;
    lz = lz + k * rz;
}

void incrementColors() {
    red += 1;
    if (red >= 255) {
        red = 0;
        green++;
        if (green == 255) {
            green = 0;
            blue++;
        }
    }
}

unsigned char* picking(int x, int y) {
    unsigned char* res = (unsigned char*)malloc(4);
    GLint viewport[4];

    //glDisable(GL_LIGHTING);
    //glDisable(GL_TEXTURE_2D);

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glDepthFunc(GL_LEQUAL);
    // Draw
    gluLookAt(camX, camY, camZ,
        0.0, 0.0, 0.0,
        0.0f, 1.0f, 0.0f);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glColorPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, NULL);

    glDepthFunc(GL_LESS);

    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, res);

    //glEnable(GL_LIGHTING);
    //glEnable(GL_TEXTURE_2D);

    return res;
}

void removeCube(unsigned int index) {
    std::vector<float> newVertices;
    std::vector<unsigned int> newIndexes;
    std::vector<float> newNormalcolors;
    std::vector<float> newColors;
    // Front
    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newIndexes.push_back(0);
    newIndexes.push_back(0);
    newIndexes.push_back(0);

    newIndexes.push_back(0);
    newIndexes.push_back(0);
    newIndexes.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    // Back
    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newIndexes.push_back(0);
    newIndexes.push_back(0);
    newIndexes.push_back(0);

    newIndexes.push_back(0);
    newIndexes.push_back(0);
    newIndexes.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    // Top
    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newIndexes.push_back(0);
    newIndexes.push_back(0);
    newIndexes.push_back(0);

    newIndexes.push_back(0);
    newIndexes.push_back(0);
    newIndexes.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    // Bottom
    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newIndexes.push_back(0);
    newIndexes.push_back(0);
    newIndexes.push_back(0);

    newIndexes.push_back(0);
    newIndexes.push_back(0);
    newIndexes.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    // Right
    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newIndexes.push_back(0);
    newIndexes.push_back(0);
    newIndexes.push_back(0);

    newIndexes.push_back(0);
    newIndexes.push_back(0);
    newIndexes.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    // Left
    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newVertices.push_back(0);
    newVertices.push_back(0);
    newVertices.push_back(0);

    newIndexes.push_back(0);
    newIndexes.push_back(0);
    newIndexes.push_back(0);

    newIndexes.push_back(0);
    newIndexes.push_back(0);
    newIndexes.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);
    newNormalcolors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    newColors.push_back(0);
    newColors.push_back(0);
    newColors.push_back(0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferSubData(GL_ARRAY_BUFFER, index * 24 * 3 * sizeof(float), newVertices.size() * sizeof(float), newVertices.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index * 36 * sizeof(int), newIndexes.size() * sizeof(int), newIndexes.data());

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferSubData(GL_ARRAY_BUFFER, index * 24 * 3 * sizeof(float), newColors.size() * sizeof(float), newColors.data());

    glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    glBufferSubData(GL_ARRAY_BUFFER, index * 24 * 3 * sizeof(float), newNormalcolors.size() * sizeof(float), newNormalcolors.data());
}

void addCube(int x, int y, int z) {
    std::vector<float> newVertices;
    std::vector<unsigned int> newIndexes;
    std::vector<float> newNormalcolors;
    std::vector<float> newColors;
    // Front
    newVertices.push_back(-0.5 + x);
    newVertices.push_back(-0.5 + y);
    newVertices.push_back(0.5 + z);

    newVertices.push_back(0.5 + x);
    newVertices.push_back(-0.5 + y);
    newVertices.push_back(0.5 + z);

    newVertices.push_back(0.5 + x);
    newVertices.push_back(0.5 + y);
    newVertices.push_back(0.5 + z);

    newVertices.push_back(-0.5 + x);
    newVertices.push_back(0.5 + y);
    newVertices.push_back(0.5 + z);

    newIndexes.push_back(indexCount + 0);
    newIndexes.push_back(indexCount + 1);
    newIndexes.push_back(indexCount + 2);

    newIndexes.push_back(indexCount + 2);
    newIndexes.push_back(indexCount + 3);
    newIndexes.push_back(indexCount + 0);

    indexCount += 4;

    float red_vec = std::get<0>(colorsVec[indColors]);
    float green_vec = std::get<1>(colorsVec[indColors]);
    float blue_vec = std::get<2>(colorsVec[indColors]);

    newNormalcolors.push_back(red_vec);
    newNormalcolors.push_back(green_vec);
    newNormalcolors.push_back(blue_vec);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    nextPosX[red][green][blue] = x;
    nextPosY[red][green][blue] = y;
    nextPosZ[red][green][blue] = z + 1;

    currPosX[red][green][blue] = x;
    currPosY[red][green][blue] = y;
    currPosZ[red][green][blue] = z;

    incrementColors();

    // Back
    newVertices.push_back(-0.5 + x);
    newVertices.push_back(-0.5 + y);
    newVertices.push_back(-0.5 + z);

    newVertices.push_back(-0.5 + x);
    newVertices.push_back(0.5 + y);
    newVertices.push_back(-0.5 + z);

    newVertices.push_back(0.5 + x);
    newVertices.push_back(0.5 + y);
    newVertices.push_back(-0.5 + z);

    newVertices.push_back(0.5 + x);
    newVertices.push_back(-0.5 + y);
    newVertices.push_back(-0.5 + z);

    newIndexes.push_back(indexCount + 0);
    newIndexes.push_back(indexCount + 1);
    newIndexes.push_back(indexCount + 2);

    newIndexes.push_back(indexCount + 2);
    newIndexes.push_back(indexCount + 3);
    newIndexes.push_back(indexCount + 0);

    indexCount += 4;

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    nextPosX[red][green][blue] = x;
    nextPosY[red][green][blue] = y;
    nextPosZ[red][green][blue] = z - 1;

    currPosX[red][green][blue] = x;
    currPosY[red][green][blue] = y;
    currPosZ[red][green][blue] = z;

    incrementColors();

    // Top
    newVertices.push_back(-0.5 + x);
    newVertices.push_back(0.5 + y);
    newVertices.push_back(-0.5 + z);

    newVertices.push_back(-0.5 + x);
    newVertices.push_back(0.5 + y);
    newVertices.push_back(0.5 + z);

    newVertices.push_back(0.5 + x);
    newVertices.push_back(0.5 + y);
    newVertices.push_back(0.5 + z);

    newVertices.push_back(0.5 + x);
    newVertices.push_back(0.5 + y);
    newVertices.push_back(-0.5 + z);

    newIndexes.push_back(indexCount + 0);
    newIndexes.push_back(indexCount + 1);
    newIndexes.push_back(indexCount + 2);

    newIndexes.push_back(indexCount + 2);
    newIndexes.push_back(indexCount + 3);
    newIndexes.push_back(indexCount + 0);

    indexCount += 4;

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    nextPosX[red][green][blue] = x;
    nextPosY[red][green][blue] = y + 1;
    nextPosZ[red][green][blue] = z;

    currPosX[red][green][blue] = x;
    currPosY[red][green][blue] = y;
    currPosZ[red][green][blue] = z;

    incrementColors();

    // Bottom
    newVertices.push_back(-0.5 + x);
    newVertices.push_back(-0.5 + y);
    newVertices.push_back(-0.5 + z);

    newVertices.push_back(0.5 + x);
    newVertices.push_back(-0.5 + y);
    newVertices.push_back(-0.5 + z);

    newVertices.push_back(0.5 + x);
    newVertices.push_back(-0.5 + y);
    newVertices.push_back(0.5 + z);

    newVertices.push_back(-0.5 + x);
    newVertices.push_back(-0.5 + y);
    newVertices.push_back(0.5 + z);

    newIndexes.push_back(indexCount + 0);
    newIndexes.push_back(indexCount + 1);
    newIndexes.push_back(indexCount + 2);

    newIndexes.push_back(indexCount + 2);
    newIndexes.push_back(indexCount + 3);
    newIndexes.push_back(indexCount + 0);

    indexCount += 4;

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    nextPosX[red][green][blue] = x;
    nextPosY[red][green][blue] = y - 1;
    nextPosZ[red][green][blue] = z - 1;

    currPosX[red][green][blue] = x;
    currPosY[red][green][blue] = y;
    currPosZ[red][green][blue] = z;

    incrementColors();

    // Right
    newVertices.push_back(0.5 + x);
    newVertices.push_back(-0.5 + y);
    newVertices.push_back(-0.5 + z);

    newVertices.push_back(0.5 + x);
    newVertices.push_back(0.5 + y);
    newVertices.push_back(-0.5 + z);

    newVertices.push_back(0.5 + x);
    newVertices.push_back(0.5 + y);
    newVertices.push_back(0.5 + z);

    newVertices.push_back(0.5 + x);
    newVertices.push_back(-0.5 + y);
    newVertices.push_back(0.5 + z);

    newIndexes.push_back(indexCount + 0);
    newIndexes.push_back(indexCount + 1);
    newIndexes.push_back(indexCount + 2);

    newIndexes.push_back(indexCount + 2);
    newIndexes.push_back(indexCount + 3);
    newIndexes.push_back(indexCount + 0);

    indexCount += 4;

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    nextPosX[red][green][blue] = x + 1;
    nextPosY[red][green][blue] = y;
    nextPosZ[red][green][blue] = z;

    currPosX[red][green][blue] = x;
    currPosY[red][green][blue] = y;
    currPosZ[red][green][blue] = z;

    incrementColors();

    // Left
    newVertices.push_back(-0.5 + x);
    newVertices.push_back(-0.5 + y);
    newVertices.push_back(-0.5 + z);

    newVertices.push_back(-0.5 + x);
    newVertices.push_back(-0.5 + y);
    newVertices.push_back(0.5 + z);

    newVertices.push_back(-0.5 + x);
    newVertices.push_back(0.5 + y);
    newVertices.push_back(0.5 + z);

    newVertices.push_back(-0.5 + x);
    newVertices.push_back(0.5 + y);
    newVertices.push_back(-0.5 + z);

    newIndexes.push_back(indexCount + 0);
    newIndexes.push_back(indexCount + 1);
    newIndexes.push_back(indexCount + 2);

    newIndexes.push_back(indexCount + 2);
    newIndexes.push_back(indexCount + 3);
    newIndexes.push_back(indexCount + 0);

    indexCount += 4;

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newNormalcolors.push_back(red / 255.0f);
    newNormalcolors.push_back(green / 255.0f);
    newNormalcolors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    newColors.push_back(red / 255.0f);
    newColors.push_back(green / 255.0f);
    newColors.push_back(blue / 255.0f);

    nextPosX[red][green][blue] = x - 1;
    nextPosY[red][green][blue] = y;
    nextPosZ[red][green][blue] = z;

    currPosX[red][green][blue] = x;
    currPosY[red][green][blue] = y;
    currPosZ[red][green][blue] = z;

    incrementColors();

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), newVertices.size() * sizeof(float), newVertices.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, numIndex * sizeof(int), newIndexes.size() * sizeof(int), newIndexes.data());

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferSubData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), newColors.size() * sizeof(float), newColors.data());

    glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    glBufferSubData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), newNormalcolors.size() * sizeof(float), newNormalcolors.data());

    vertexCount += 6 * 4;
    numIndex += 36;

    mapPointIndex[std::make_tuple(x, y, z)] = cubeIndex;

    cubeIndex++;
}

void processMouseButtons(int button, int state, int xx, int yy)
{
    printf("%d %d\n", xx, yy);
    if (state == GLUT_DOWN) {
        if (button == GLUT_MIDDLE_BUTTON) {
            unsigned char* result = picking(xx, yy);
            if (result[0] != 0 || result[1] != 0 || result[2] != 0) {
                printf("Mode: %d\n", minecraftMode);
                if (minecraftMode == 0) { // Construct
                    printf("Picked Color %u %u %u\n", result[0], result[1], result[2]);
                    printf("Bloco deve ser colocado em %f %f %f", nextPosX[result[0]][result[1]][result[2]],
                        nextPosY[result[0]][result[1]][result[2]],
                        nextPosZ[result[0]][result[1]][result[2]]);
                    addCube(nextPosX[result[0]][result[1]][result[2]],
                        nextPosY[result[0]][result[1]][result[2]],
                        nextPosZ[result[0]][result[1]][result[2]]);
                }
                else { // Destruct
                    printf("Picked Color %u %u %u\n", result[0], result[1], result[2]);
                    printf("Bloco deve ser removido em %f %f %f", currPosX[result[0]][result[1]][result[2]],
                        currPosY[result[0]][result[1]][result[2]],
                        currPosZ[result[0]][result[1]][result[2]]);
                    removeCube(mapPointIndex[std::make_tuple(
                        currPosX[result[0]][result[1]][result[2]],
                        currPosY[result[0]][result[1]][result[2]],
                        currPosZ[result[0]][result[1]][result[2]])]);
                }
            }
            else
                printf("Nothing selected\n");
        }

        startX = xx;
        startY = yy;
        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
    }
    else if (state == GLUT_UP) {
        if (tracking == 1) {
            alpha += (xx - startX);
            beta += (yy - startY);
        }
        else if (tracking == 2) {

            r -= yy - startY;
            if (r < 3)
                r = 3.0;
        }
        tracking = 0;
    }
}


void processMouseMotion(int xx, int yy)
{

    int deltaX, deltaY;
    int alphaAux, betaAux;
    int rAux;

    if (!tracking)
        return;

    deltaX = xx - startX;
    deltaY = yy - startY;

    if (tracking == 1) {


        alphaAux = alpha + deltaX;
        betaAux = beta + deltaY;

        if (betaAux > 85.0)
            betaAux = 85.0;
        else if (betaAux < -85.0)
            betaAux = -85.0;

        rAux = r;
    }
    else if (tracking == 2) {

        alphaAux = alpha;
        betaAux = beta;
        rAux = r - deltaY;
        if (rAux < 3)
            rAux = 3;
    }
    camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camY = rAux * sin(betaAux * 3.14 / 180.0);

    glutPostRedisplay();
}

void changeSize(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you can�t make a window with zero width).
    if (h == 0)
        h = 1;
    // compute window's aspect ratio
    float ratio = w * 1.0f / h;
    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load the identity matrix
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    // Set the perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}


void renderScene(void)
{

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    if (!(abs(camX) > 127.5f || abs(camZ) > 127.5f)) {
        camY = getHeightf(camX, camZ) + 3;
        aux1();
    }
    gluLookAt(camX, camY, camZ,
        lx, ly, lz,
        0.0f, 1.0f, 0.0f);

    //glColor3f(1,0,0);
    // put drawing instructions here
    //glutSolidCube(1);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    glColorPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, NULL);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    float fps;
    int time;
    char s[64];

    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        fps = frame * 1000.0 / (time - timebase);
        timebase = time;
        frame = 0;
        sprintf(s, "FPS: %f6.2", fps);
        glutSetWindowTitle(s);
    }

    // End of frame
    glutSwapBuffers();
}

void drawCube(int x, int y, int z) {
    // Front
    vertices.push_back(-0.5 + x);
    vertices.push_back(-0.5 + y);
    vertices.push_back(0.5 + z);

    vertices.push_back(0.5 + x);
    vertices.push_back(-0.5 + y);
    vertices.push_back(0.5 + z);

    vertices.push_back(0.5 + x);
    vertices.push_back(0.5 + y);
    vertices.push_back(0.5 + z);

    vertices.push_back(-0.5 + x);
    vertices.push_back(0.5 + y);
    vertices.push_back(0.5 + z);

    indexes.push_back(indexCount + 0);
    indexes.push_back(indexCount + 1);
    indexes.push_back(indexCount + 2);

    indexes.push_back(indexCount + 2);
    indexes.push_back(indexCount + 3);
    indexes.push_back(indexCount + 0);

    indexCount += 4;

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    nextPosX[red][green][blue] = x;
    nextPosY[red][green][blue] = y;
    nextPosZ[red][green][blue] = z + 1;

    currPosX[red][green][blue] = x;
    currPosY[red][green][blue] = y;
    currPosZ[red][green][blue] = z;

    incrementColors();

    // Back
    vertices.push_back(-0.5 + x);
    vertices.push_back(-0.5 + y);
    vertices.push_back(-0.5 + z);

    vertices.push_back(-0.5 + x);
    vertices.push_back(0.5 + y);
    vertices.push_back(-0.5 + z);

    vertices.push_back(0.5 + x);
    vertices.push_back(0.5 + y);
    vertices.push_back(-0.5 + z);

    vertices.push_back(0.5 + x);
    vertices.push_back(-0.5 + y);
    vertices.push_back(-0.5 + z);

    indexes.push_back(indexCount + 0);
    indexes.push_back(indexCount + 1);
    indexes.push_back(indexCount + 2);

    indexes.push_back(indexCount + 2);
    indexes.push_back(indexCount + 3);
    indexes.push_back(indexCount + 0);

    indexCount += 4;

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    nextPosX[red][green][blue] = x;
    nextPosY[red][green][blue] = y;
    nextPosZ[red][green][blue] = z - 1;

    currPosX[red][green][blue] = x;
    currPosY[red][green][blue] = y;
    currPosZ[red][green][blue] = z;

    incrementColors();

    // Top
    vertices.push_back(-0.5 + x);
    vertices.push_back(0.5 + y);
    vertices.push_back(-0.5 + z);

    vertices.push_back(-0.5 + x);
    vertices.push_back(0.5 + y);
    vertices.push_back(0.5 + z);

    vertices.push_back(0.5 + x);
    vertices.push_back(0.5 + y);
    vertices.push_back(0.5 + z);

    vertices.push_back(0.5 + x);
    vertices.push_back(0.5 + y);
    vertices.push_back(-0.5 + z);

    indexes.push_back(indexCount + 0);
    indexes.push_back(indexCount + 1);
    indexes.push_back(indexCount + 2);

    indexes.push_back(indexCount + 2);
    indexes.push_back(indexCount + 3);
    indexes.push_back(indexCount + 0);

    indexCount += 4;

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    nextPosX[red][green][blue] = x;
    nextPosY[red][green][blue] = y + 1;
    nextPosZ[red][green][blue] = z;

    currPosX[red][green][blue] = x;
    currPosY[red][green][blue] = y;
    currPosZ[red][green][blue] = z;

    incrementColors();

    // Bottom
    vertices.push_back(-0.5 + x);
    vertices.push_back(-0.5 + y);
    vertices.push_back(-0.5 + z);

    vertices.push_back(0.5 + x);
    vertices.push_back(-0.5 + y);
    vertices.push_back(-0.5 + z);

    vertices.push_back(0.5 + x);
    vertices.push_back(-0.5 + y);
    vertices.push_back(0.5 + z);

    vertices.push_back(-0.5 + x);
    vertices.push_back(-0.5 + y);
    vertices.push_back(0.5 + z);

    indexes.push_back(indexCount + 0);
    indexes.push_back(indexCount + 1);
    indexes.push_back(indexCount + 2);

    indexes.push_back(indexCount + 2);
    indexes.push_back(indexCount + 3);
    indexes.push_back(indexCount + 0);

    indexCount += 4;

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    nextPosX[red][green][blue] = x;
    nextPosY[red][green][blue] = y - 1;
    nextPosZ[red][green][blue] = z - 1;

    currPosX[red][green][blue] = x;
    currPosY[red][green][blue] = y;
    currPosZ[red][green][blue] = z;

    incrementColors();

    // Right
    vertices.push_back(0.5 + x);
    vertices.push_back(-0.5 + y);
    vertices.push_back(-0.5 + z);

    vertices.push_back(0.5 + x);
    vertices.push_back(0.5 + y);
    vertices.push_back(-0.5 + z);

    vertices.push_back(0.5 + x);
    vertices.push_back(0.5 + y);
    vertices.push_back(0.5 + z);

    vertices.push_back(0.5 + x);
    vertices.push_back(-0.5 + y);
    vertices.push_back(0.5 + z);

    indexes.push_back(indexCount + 0);
    indexes.push_back(indexCount + 1);
    indexes.push_back(indexCount + 2);

    indexes.push_back(indexCount + 2);
    indexes.push_back(indexCount + 3);
    indexes.push_back(indexCount + 0);

    indexCount += 4;

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    nextPosX[red][green][blue] = x + 1;
    nextPosY[red][green][blue] = y;
    nextPosZ[red][green][blue] = z;

    currPosX[red][green][blue] = x;
    currPosY[red][green][blue] = y;
    currPosZ[red][green][blue] = z;

    incrementColors();

    // Left
    vertices.push_back(-0.5 + x);
    vertices.push_back(-0.5 + y);
    vertices.push_back(-0.5 + z);

    vertices.push_back(-0.5 + x);
    vertices.push_back(-0.5 + y);
    vertices.push_back(0.5 + z);

    vertices.push_back(-0.5 + x);
    vertices.push_back(0.5 + y);
    vertices.push_back(0.5 + z);

    vertices.push_back(-0.5 + x);
    vertices.push_back(0.5 + y);
    vertices.push_back(-0.5 + z);

    indexes.push_back(indexCount + 0);
    indexes.push_back(indexCount + 1);
    indexes.push_back(indexCount + 2);

    indexes.push_back(indexCount + 2);
    indexes.push_back(indexCount + 3);
    indexes.push_back(indexCount + 0);

    indexCount += 4;

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    normalColors.push_back(red / 255.0f);
    normalColors.push_back(green / 255.0f);
    normalColors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    colors.push_back(red / 255.0f);
    colors.push_back(green / 255.0f);
    colors.push_back(blue / 255.0f);

    nextPosX[red][green][blue] = x - 1;
    nextPosY[red][green][blue] = y;
    nextPosZ[red][green][blue] = z;

    currPosX[red][green][blue] = x;
    currPosY[red][green][blue] = y;
    currPosZ[red][green][blue] = z;

    numIndex += 36;
    mapPointIndex[std::make_tuple(x, y, z)] = cubeIndex;
    cubeIndex++;
    incrementColors();
}

void renderText() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    // set projection so that coordinates match window pixels
    gluOrtho2D(0, tw, 0, th);
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_DEPTH_TEST);

    glPushMatrix();
    glLoadIdentity();
    glRasterPos2d(2, 2); // text position in pixels

    glColor3f(std::get<0>(colorsVec[indColors]), std::get<1>(colorsVec[indColors]), std::get<2>(colorsVec[indColors]));

    for (const unsigned char* c = "COLOR"; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
}

void init() {

    // 	Load the height map "terreno.jpg"
    ilGenImages(1, &t);
    ilBindImage(t);

    // terreno.jpg is the image containing our height map
    ilLoadImage((ILstring)"../tnt.png");

    // convert the image to single channel per pixel
    // with values ranging between 0 and 255
    ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);

    // important: check tw and th values
    // both should be equal to 256
    // if not there was an error loading the image
    // most likely the image could not be found
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    std::cout << "Largura: " << tw << " Altura: " << th << std::endl;

    // imageData is a LINEAR array with the pixel values
    imageData = ilGetData();

    int h = (int)th - 1, w = (int)tw - 1;

    glGenBuffers(4, buffers);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    int side = h;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            for(int k=getHeight(i,j)-8; k < getHeight(i,j); k++) drawCube(i-w/2,k, j-h/2);
        }
    }

    vertexCount += side * side * 6 * 4;

    int maxPlacedBlocks = 1000;

    for (int i = 0; i < maxPlacedBlocks; i++) {
        for (int j = 0; j < 24; j++) {
            vertices.push_back(0);
            colors.push_back(0);
            normalColors.push_back(0);
        }

        for (int k = 0; k < 12; k++) {
            indexes.push_back(0);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(int), indexes.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    glBufferData(GL_ARRAY_BUFFER, normalColors.size() * sizeof(float), normalColors.data(), GL_STATIC_DRAW);
}

void processKeys(unsigned char key, int xx, int yy) {
    if (key == 'W' || key == 'w') {
        if (k < 0) {
            k = -k;
        }
        aux();
    }
    else if (key == 'A' || key == 'a') {
        if (k > 0) {
            k = -k;
        }
        produtoExterno();
    }
    else if (key == 'S' || key == 's') {
        if (k > 0) {
            k = -k;
        }
        aux();

    }
    else if (key == 'D' || key == 'd') {
        if (k < 0) {
            k = -k;
        }
        produtoExterno();
    }
    else if (key == 'C' || key == 'c') {
        alpha += 0.05;
    }
    else if (key == 'M' || key == 'm'){
        if (minecraftMode == 0) {
            minecraftMode = 1;
        }
        else minecraftMode = 0;

    }
    else if (key == 'V' || key == 'v') {
        indColors = (indColors + 1) % colorsVec.size();
    }
}


int main(int argc, char** argv)
{
    // put GLUT�s init here
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG@DI");

    // put callback registry here
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutDisplayFunc(renderScene);
    glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);

    colorsVec.push_back(std::make_tuple(1, 0, 0)); // RED
    colorsVec.push_back(std::make_tuple(0, 1, 0)); // GREEN
    colorsVec.push_back(std::make_tuple(0, 0, 1)); // BLUE

    glutKeyboardFunc(processKeys);

    // some OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT, GL_FILL);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glewInit();
    ilInit();
    init();

    // enter GLUT�s main cycle
    glutMainLoop();

    return 1;
}