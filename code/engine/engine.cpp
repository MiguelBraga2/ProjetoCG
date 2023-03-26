#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <list>
#include <iostream>
#include <string>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "../libraries/tinyxml2.h"
#include "../shared/triangle.hpp"
#include "../shared/IO.hpp"
#include "camera.hpp"
#include "group.hpp"


using namespace tinyxml2;
using namespace std;

float width, height;
Camera* camera;
Group* group;

// For FPS count
int timebase;
float frames;


void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(camera->getFov() ,ratio, camera->getNear() , camera->getFar());
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
    if (camera->getMode() == 0){
        gluLookAt(camera->getPosition().getX(),camera->getPosition().getY(),camera->getPosition().getZ(),
                  camera->getLookAtPosition().getX() ,camera->getLookAtPosition().getY(),camera->getLookAtPosition().getZ(),
                  camera->getUpVector().getX(),camera->getUpVector().getY(),camera->getUpVector().getZ());
    } else if (camera->getMode() == 1){
        gluLookAt(camera->getPosition().getX(),camera->getPosition().getY(),camera->getPosition().getZ(),
                  camera->getPosition().getX() + camera->getD().getX(),camera->getPosition().getY() + camera->getD().getY(),camera->getPosition().getZ() + camera->getD().getZ(),
                  camera->getUpVector().getX(),camera->getUpVector().getY(),camera->getUpVector().getZ());
    }

    // put axis drawing in here
    glBegin(GL_LINES);

    // X-axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(
            -100.0f, 0.0f, 0.0f);
    glVertex3f( 100.0f, 0.0f, 0.0f);

    // Y Axis in Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f,
               -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);

    // Z Axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f,
               -100.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
     
    // transformation and drawing instructions here
    group->drawGroup(1, 1, 1);

    frames++;
    int time = glutGet(GLUT_ELAPSED_TIME);
    int fps;
    if(time-timebase > 1000){
        fps = frames * 1000.0/(time-timebase);
        timebase = time;
        frames = 0;
        char* s = (char*)malloc(4);
        std::sprintf(s, "%d", fps);
        glutSetWindowTitle(s);
    }


    // End of frame
    glutSwapBuffers();
}

// write function to process keyboard events

void keyboard_events(unsigned char key, int x, int y) {
    if (key == 'w'){
        camera->moveForwards();
    }
    else if (key == 's'){
        camera->moveBackwards();
    }/*
    else if (key == 'a'){
        float* crossP = (float*) malloc(3*sizeof(float));
        crossProduct(0, 1, 0, cameraLookAtX, cameraLookAtY, cameraLookAtZ, crossP);
        cameraPositionX += crossP[0];
        cameraPositionY += crossP[1];
        cameraPositionZ += crossP[2];
    }
    else if (key == 'd'){
        float* crossP = (float*) malloc(3*sizeof(float));
        crossProduct(0, 1, 0, cameraLookAtX, cameraLookAtY, cameraLookAtZ, crossP);
        cameraPositionX -= crossP[0];
        cameraPositionY -= crossP[1];
        cameraPositionZ -= crossP[2];
    }*/

    if (key == '2'){
        camera->incrementIncrement();
    }
    else if (key == '1'){
        camera->decrementIncrement();
    }
    else if (key == 'm'){
        camera->changeMode();
    }

    glutPostRedisplay();
}

/*
void updateCamera(){
    cameraLookAtX = cos(rotationBeta)*sin(rotationAlpha);
    cameraLookAtY = sin(rotationBeta);
    cameraLookAtZ = cos(rotationAlpha)*cos(rotationBeta);

    float lookAtLength = sqrt(pow(cameraLookAtX, 2) + pow(cameraLookAtY, 2) + pow(cameraLookAtZ, 2));
    cameraLookAtX = cameraLookAtX / lookAtLength;
    cameraLookAtY = cameraLookAtY / lookAtLength;
    cameraLookAtZ = cameraLookAtZ / lookAtLength;

    glutPostRedisplay();
}*/

void processSpecialKeys(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            camera->incrementAlfa(); break;

        case GLUT_KEY_LEFT:
            camera->decrementAlfa(); break;

        case GLUT_KEY_UP:
            camera->incrementBeta(); break;

        case GLUT_KEY_DOWN:
            camera->decrementBeta(); break;

        case GLUT_KEY_PAGE_DOWN:
            camera->decrementRadius(); break;

        case GLUT_KEY_PAGE_UP: camera->incrementRadius(); break;
    }

    glutPostRedisplay();
}

int readXML(char* filePath){
    XMLDocument *doc = new XMLDocument();
    XMLError error = doc->LoadFile(filePath);
    XMLNode* world = doc->FirstChildElement("world");

    float fov = 0, far = 0, near = 0;
    Point* cameraPosition = new Point(), * cameraLookAt = new Point(), * cameraUpVector = new Point();
  
    if (!error && world) {
        /* window */
        XMLElement* windowElem = world->FirstChildElement("window");
        if (windowElem) {
            width =  stoi(windowElem->Attribute("width"));
            height = stoi(windowElem->Attribute("height"));
        }
        
        /* camera */
        XMLElement* cameraElem = world->FirstChildElement("camera");
        if (cameraElem) {
            
            /* camera position */
            XMLElement* position = cameraElem->FirstChildElement("position");
            if (position) {
                cameraPosition->setX(stof(position->Attribute("x")));
                cameraPosition->setY(stof(position->Attribute("y")));
                cameraPosition->setZ(stof(position->Attribute("z")));
            }

            /* camera lookAt */
            XMLElement* lookAt = cameraElem->FirstChildElement("lookAt");
            if (lookAt) {
                cameraLookAt->setX(stof(lookAt->Attribute("x")));
                cameraLookAt->setY(stof(lookAt->Attribute("y")));
                cameraLookAt->setZ(stof(lookAt->Attribute("z")));
            }

            /* camera up */
            XMLElement* up = cameraElem->FirstChildElement("up");
            if (up) {
                cameraUpVector->setX(stof(up->Attribute("x")));
                cameraUpVector->setY(stof(up->Attribute("y")));
                cameraUpVector->setZ(stof(up->Attribute("z")));
            }
            
            /* camera projection */
            XMLElement* projection = cameraElem->FirstChildElement("projection");
            if (projection) {
                fov = stof(projection->Attribute("fov"));
                near = stof(projection->Attribute("near"));
                far = stof(projection->Attribute("far"));
            }
          
            camera = new Camera(*cameraPosition, *cameraLookAt, *cameraUpVector, fov, near, far);
        }

        group = new Group();
        group->readXML(world->FirstChildElement("group"));
    }

    delete doc;
    
    return (int) error;
}

int main(int argc, char **argv) {
    if (argc == 2) {

        int error = readXML(argv[1]);

        if (!error) {

            // init GLUT and the window
            glutInit(&argc, argv);
            glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
            glutInitWindowPosition(100, 100);
            glutInitWindowSize(width, height);
            glutCreateWindow("ProjetoCG");

            timebase = glutGet(GLUT_ELAPSED_TIME);

            // Required callback registry 
            glutIdleFunc(renderScene);
            glutDisplayFunc(renderScene);
            glutReshapeFunc(changeSize);


            // put here the registration of the keyboard callbacks
            glutSpecialFunc(processSpecialKeys);
            glutKeyboardFunc(keyboard_events);
            //glutPassiveMotionFunc(mouse_function);

            // Required for VBOs 
            glewInit();
            glEnableClientState(GL_VERTEX_ARRAY);

            group->prepareBuffers();

            // 	OpenGL settings
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT, GL_LINE);

            // enter GLUT's main cycle
            glutMainLoop();
        }
        else {
            cout << "Ficheiro não encontrado ou erro na sua leitura." << endl;
        }
    }
    else {
        cout << "Número incorreto de argumentos." << endl;
    }
	
	return 1;
}
