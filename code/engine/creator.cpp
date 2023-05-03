#include "creator.h"

#include <IL/il.h>
#include <iostream>

/////// MINECRAFT /////////

unsigned char* imageData;

unsigned int t, tw, th;

float getHeight(int i, int j) {
    return imageData[i * tw + j];
}

Creator::Creator(Camera* camera) {
    this->globalCamera = camera;
    ilInit();

    colorsVec.push_back(std::make_tuple(1, 0, 0)); // RED
    colorsVec.push_back(std::make_tuple(0, 1, 0)); // GREEN
    colorsVec.push_back(std::make_tuple(0, 0, 1)); // BLUE
    colorsVec.push_back(std::make_tuple(1, 1, 0.1)); // LASER LEMON
    colorsVec.push_back(std::make_tuple(1, 0.3, 1)); // ULTRA PINK
    colorsVec.push_back(std::make_tuple(0, 0, 0)); // BLACK
    colorsVec.push_back(std::make_tuple(1, 1, 1)); // WHITE
    colorsVec.push_back(std::make_tuple(0, 0.2, 0)); // DARK GREEN
    colorsVec.push_back(std::make_tuple(0.2, 0.07, 0)); // ZINNWALDITE BROWN

    // 	Load the height map "terreno.jpg"
    ilGenImages(1, &t);
    ilBindImage(t);

    // terreno.jpg is the image containing our height map
    ilLoadImage((ILstring)"../terreno.jpg");

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

    int side = h;

    glGenBuffers(4, buffers);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            drawCube(i,j, getHeight(i,j)/2);
            //float height = getHeight(i,j)/2;
            //for(int k=height-3; k < height; k++) drawCube(i-w/2,k, j-h/2);
        }
    }

    int maxPlacedBlocks = 1000;

    vertexCount += side * side * 6 * 4;

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

void Creator::incrementColors() {
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

void Creator::drawCube(int x, int y, int z) {
    float newVertices[] = {
            -0.5f + x, -0.5f + y, 0.5f + z, // FRONT
            0.5f + x, -0.5f + y, 0.5f + z,
            0.5f + x, 0.5f + y, 0.5f + z,
            -0.5f + x, 0.5f + y, 0.5f + z,
            -0.5f + x, -0.5f + y, -0.5f + z, // BACK
            -0.5f + x, 0.5f + y, -0.5f + z,
            0.5f + x, 0.5f + y, -0.5f + z,
            0.5f + x, -0.5f + y, -0.5f + z,
            -0.5f + x, 0.5f + y, -0.5f + z, // TOP
            -0.5f + x, 0.5f + y, 0.5f + z,
            0.5f + x, 0.5f + y, 0.5f + z,
            0.5f + x, 0.5f + y, -0.5f + z,
            -0.5f + x, -0.5f + y, -0.5f + z, // BOTTOM
            0.5f + x, -0.5f + y, -0.5f + z,
            0.5f + x, -0.5f + y, 0.5f + z,
            -0.5f + x, -0.5f + y, 0.5f + z,
            0.5f + x, -0.5f + y, -0.5f + z, // RIGHT
            0.5f + x, 0.5f + y, -0.5f + z,
            0.5f + x, 0.5f + y, 0.5f + z,
            0.5f + x, -0.5f + y, 0.5f + z,
            -0.5f + x, -0.5f + y, -0.5f + z, // LEFT
            -0.5f + x, -0.5f + y, 0.5f + z,
            -0.5f + x, 0.5f + y, 0.5f + z,
            -0.5f + x, 0.5f + y, -0.5f + z
    };

    int newIndexes[] = {
            indexCount + 0, indexCount + 1, indexCount + 2, // FRONT
            indexCount + 2, indexCount + 3, indexCount + 0,
            indexCount + 0+4, indexCount + 1+4, indexCount + 2+4, // BACK
            indexCount + 2+4, indexCount + 3+4, indexCount + 0+4,
            indexCount + 0+8, indexCount + 1+8, indexCount + 2+8, // TOP
            indexCount + 2+8, indexCount + 3+8, indexCount + 0+8,
            indexCount + 0+12, indexCount + 1+12, indexCount + 2+12, // BOTTOM
            indexCount + 2+12, indexCount + 3+12, indexCount + 0+12,
            indexCount + 0+16, indexCount + 1+16, indexCount + 2+16, // RIGHT
            indexCount + 2+16, indexCount + 3, indexCount + 0,
            indexCount + 0+20, indexCount + 1+20, indexCount + 2+20, // LEFT
            indexCount + 2+20, indexCount + 3+20, indexCount + 0+20,
    }; // Foreach face, the indexCount is increment 4 unities

    indexCount += 24;

    float red_vec = std::get<0>(colorsVec[indColors]);
    float green_vec = std::get<1>(colorsVec[indColors]);
    float blue_vec = std::get<2>(colorsVec[indColors]);

    float newNormalColors[4*6*3];
    // 4 vertices in each face, 6 faces, 3 colors per vector
    for(int i=0; i<4*6*3; i+=3){
        newNormalColors[i] = red_vec;
        newNormalColors[i+1] = green_vec;
        newNormalColors[i+2] = blue_vec;
    } // All vertices with the same color

    float newColors[4*6*3];
    // 4 vertices in each face, 6 faces, 3 colors per vector

    for(int i=0; i<4*6*3; i+=12){
        for(int j=0; j<4; j++){
            newColors[i+3*j] = red / 255.0f;
            newColors[i+3*j+1] = green / 255.0f;
            newColors[i+3*j+2] = blue / 255.0f;
        }

        int nextX = x, nextY = y, nextZ = z;

        switch(i/12){
            case 0: // FRONT
                nextZ = z+1;
                break;
            case 1: // BACK
                nextZ = z-1;
                break;
            case 2: // TOP
                nextY = y+1;
                break;
            case 3: // BOTTOM
                nextY = y-1;
                break;
            case 4: // RIGHT
                nextX = x+1;
                break;
            case 5: // LEFT
                nextX = x-1;
                break;
        }

        nextPosX[red][green][blue] = nextX;
        nextPosY[red][green][blue] = nextY;
        nextPosZ[red][green][blue] = nextZ;

        currPosX[red][green][blue] = x;
        currPosY[red][green][blue] = y;
        currPosZ[red][green][blue] = z;

        incrementColors();
    }

    for(float v: newVertices){
        vertices.push_back(v);
    }

    for(int ind: newIndexes){
        indexes.push_back(ind);
    }

    for(float normalcolor: newNormalColors){
        normalColors.push_back(normalcolor);
    }

    for(float color: newColors){
        colors.push_back(color);
    }

    numIndex += 36;
    mapPointIndex[std::make_tuple(x, y, z)] = cubeIndex;
    cubeIndex++;
    incrementColors();
}

void Creator::addCube(int x, int y, int z) {
    float newVertices[] = {
            -0.5f + x, -0.5f + y, 0.5f + z, // FRONT
            0.5f + x, -0.5f + y, 0.5f + z,
            0.5f + x, 0.5f + y, 0.5f + z,
            -0.5f + x, 0.5f + y, 0.5f + z,
            -0.5f + x, -0.5f + y, -0.5f + z, // BACK
            -0.5f + x, 0.5f + y, -0.5f + z,
            0.5f + x, 0.5f + y, -0.5f + z,
            0.5f + x, -0.5f + y, -0.5f + z,
            -0.5f + x, 0.5f + y, -0.5f + z, // TOP
            -0.5f + x, 0.5f + y, 0.5f + z,
            0.5f + x, 0.5f + y, 0.5f + z,
            0.5f + x, 0.5f + y, -0.5f + z,
            -0.5f + x, -0.5f + y, -0.5f + z, // BOTTOM
            0.5f + x, -0.5f + y, -0.5f + z,
            0.5f + x, -0.5f + y, 0.5f + z,
            -0.5f + x, -0.5f + y, 0.5f + z,
            0.5f + x, -0.5f + y, -0.5f + z, // RIGHT
            0.5f + x, 0.5f + y, -0.5f + z,
            0.5f + x, 0.5f + y, 0.5f + z,
            0.5f + x, -0.5f + y, 0.5f + z,
            -0.5f + x, -0.5f + y, -0.5f + z, // LEFT
            -0.5f + x, -0.5f + y, 0.5f + z,
            -0.5f + x, 0.5f + y, 0.5f + z,
            -0.5f + x, 0.5f + y, -0.5f + z
    };

    int newIndexes[] = {
            indexCount + 0, indexCount + 1, indexCount + 2, // FRONT
            indexCount + 2, indexCount + 3, indexCount + 0,
            indexCount + 0+4, indexCount + 1+4, indexCount + 2+4, // BACK
            indexCount + 2+4, indexCount + 3+4, indexCount + 0+4,
            indexCount + 0+8, indexCount + 1+8, indexCount + 2+8, // TOP
            indexCount + 2+8, indexCount + 3+8, indexCount + 0+8,
            indexCount + 0+12, indexCount + 1+12, indexCount + 2+12, // BOTTOM
            indexCount + 2+12, indexCount + 3+12, indexCount + 0+12,
            indexCount + 0+16, indexCount + 1+16, indexCount + 2+16, // RIGHT
            indexCount + 2+16, indexCount + 3, indexCount + 0,
            indexCount + 0+20, indexCount + 1+20, indexCount + 2+20, // LEFT
            indexCount + 2+20, indexCount + 3+20, indexCount + 0+20,
    }; // Foreach face, the indexCount is increment 4 unities

    indexCount += 24;

    float red_vec = std::get<0>(colorsVec[indColors]);
    float green_vec = std::get<1>(colorsVec[indColors]);
    float blue_vec = std::get<2>(colorsVec[indColors]);

    float newNormalColors[4*6*3];
    // 4 vertices in each face, 6 faces, 3 colors per vector
    for(int i=0; i<4*6*3; i+=3){
        newNormalColors[i] = red_vec;
        newNormalColors[i+1] = green_vec;
        newNormalColors[i+2] = blue_vec;
    } // All vertices with the same color

    float newColors[4*6*3];
    // 4 vertices in each face, 6 faces, 3 colors per vector

    for(int i=0; i<4*6*3; i+=12){
        for(int j=0; j<4; j++){
            newColors[i+3*j] = red / 255.0f;
            newColors[i+3*j+1] = green / 255.0f;
            newColors[i+3*j+2] = blue / 255.0f;
        }

        int nextX = x, nextY = y, nextZ = z;

        switch(i/12){
            case 0: // FRONT
                nextZ = z+1;
                break;
            case 1: // BACK
                nextZ = z-1;
                break;
            case 2: // TOP
                nextY = y+1;
                break;
            case 3: // BOTTOM
                nextY = y-1;
                break;
            case 4: // RIGHT
                nextX = x+1;
                break;
            case 5: // LEFT
                nextX = x-1;
                break;
        }

        nextPosX[red][green][blue] = nextX;
        nextPosY[red][green][blue] = nextY;
        nextPosZ[red][green][blue] = nextZ;

        currPosX[red][green][blue] = x;
        currPosY[red][green][blue] = y;
        currPosZ[red][green][blue] = z;

        incrementColors();
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), sizeof(newVertices), newVertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, numIndex * sizeof(int), sizeof(newIndexes), newIndexes);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferSubData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), sizeof(newColors), newColors);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    glBufferSubData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), sizeof(newNormalColors), newNormalColors);

    vertexCount += 6 * 4;
    numIndex += 36;

    mapPointIndex[std::make_tuple(x, y, z)] = cubeIndex;

    cubeIndex++;
}

void Creator::removeCube(unsigned int index) {
    GLfloat newVertices[] = {0,0,0,0,0,0,0,0,0,0,0,0, // Front
                             0,0,0,0,0,0,0,0,0,0,0,0, // Back
                             0,0,0,0,0,0,0,0,0,0,0,0, // Top
                             0,0,0,0,0,0,0,0,0,0,0,0, // Bottom
                             0,0,0,0,0,0,0,0,0,0,0,0, // Right
                             0,0,0,0,0,0,0,0,0,0,0,0}; // Left

    unsigned int newIndexes[] = {0,0,0,0,0,0, // Front
                                 0,0,0,0,0,0, // Back
                                 0,0,0,0,0,0, // Top
                                 0,0,0,0,0,0, // Bottom
                                 0,0,0,0,0,0, // Right
                                 0,0,0,0,0,0}; // Left

    float newNormalColors[] = {0,0,0,0,0,0,0,0,0,0,0,0, // Front
                               0,0,0,0,0,0,0,0,0,0,0,0, // Back
                               0,0,0,0,0,0,0,0,0,0,0,0, // Top
                               0,0,0,0,0,0,0,0,0,0,0,0, // Bottom
                               0,0,0,0,0,0,0,0,0,0,0,0, // Right
                               0,0,0,0,0,0,0,0,0,0,0,0}; // Left

    float newColors[] = {0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0};

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferSubData(GL_ARRAY_BUFFER, index * 24 * 3 * sizeof(float), sizeof(newVertices), newVertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index * 36 * sizeof(int), sizeof(newIndexes), newIndexes);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferSubData(GL_ARRAY_BUFFER, index * 24 * 3 * sizeof(float), sizeof(newColors), newColors);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
    glBufferSubData(GL_ARRAY_BUFFER, index * 24 * 3 * sizeof(float), sizeof(newNormalColors), newNormalColors);
}

void Creator::render(){
    glClearColor(0, 0, 0, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    globalCamera->placeCamera();

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

    // End of frame
    glutSwapBuffers();
}

unsigned char* Creator::picking(int x, int y) {
    unsigned char* res = (unsigned char*)malloc(4);
    GLint viewport[4];

    //glDisable(GL_LIGHTING);
    //glDisable(GL_TEXTURE_2D);

    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glDepthFunc(GL_LEQUAL);
    // Draw
    globalCamera->placeCamera();

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

void Creator::processMouseButtons(int button, int state, int xx, int yy)
{
    printf("%d %d\n", xx, yy);
    if (state == GLUT_DOWN) {
        if (button == GLUT_MIDDLE_BUTTON) {
            unsigned char *result = picking(xx, yy);
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
                } else { // Destruct
                    printf("Picked Color %u %u %u\n", result[0], result[1], result[2]);
                    printf("Bloco deve ser removido em %f %f %f", currPosX[result[0]][result[1]][result[2]],
                           currPosY[result[0]][result[1]][result[2]],
                           currPosZ[result[0]][result[1]][result[2]]);
                    removeCube(mapPointIndex[std::make_tuple(
                            currPosX[result[0]][result[1]][result[2]],
                            currPosY[result[0]][result[1]][result[2]],
                            currPosZ[result[0]][result[1]][result[2]])]);
                }
            } else
                printf("Nothing selected\n");
        }
    }
}