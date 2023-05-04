#ifndef ENGINE_CREATOR_H
#define ENGINE_CREATOR_H

#include <tuple>
#include <vector>
#include <map>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include "camera.hpp"

#endif

class Creator {
private:
    int red = 0, green = 0, blue = 0;

    int indColors = 0;

    std::vector<std::tuple<float, float, float>> colorsVec;

    int minecraftMode = 0; // 0 - Construir, 1 - Destruir

    GLuint buffers[4];

    std::map<std::tuple<float, float, float>, unsigned int> mapPointIndex;

    std::vector<float> vertices;
    std::vector<unsigned int> indexes;
    std::vector<float> colors;
    std::vector<float> normalColors;

    std::vector<float> cubesPositions;
    std::vector<float> cubesColors;

    float nextPosX[255][255][255];
    float nextPosY[255][255][255];
    float nextPosZ[255][255][255];

    float currPosX[255][255][255];
    float currPosY[255][255][255];
    float currPosZ[255][255][255];

    int vertexCount = 0, indexCount = 0, numIndex = 0, cubeIndex = 0;

    Camera* globalCamera;
public:
    Creator(Camera* camera);
    ~Creator();
    void drawCube(int x, int y, int z, float cRed, float cGreen, float cBlue);
    void addCube(int x, int y, int z);
    void removeCube(unsigned int index);
    void render(int height, int width);
    void processMouseButtons(int button, int state, int xx, int yy);
    unsigned char* picking(int x, int y);
    void changeBlockColor();
    void importScene(string filename);
    void exportScene(string filename);
private:
    void incrementColors();
    void renderText(int height, int width);
};


#endif //ENGINE_CREATOR_H
