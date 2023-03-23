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
float angle = 0;
Camera* camera;
Group* group;


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

/**
 * Draw a triangle, using the order specified by the indexes
 * @param t 3 indexes, specifying the positions of the triangle vertices in the points vector
 * @param red red color setting
 * @param green green color setting
 * @param blue blue color setting
 * @param points the set of all the points in the figure
 */
void drawTriangle(Triangle t, float red, float green, float blue, vector<Point> points){
    int i1 = t.getIndP1();
    int i2 = t.getIndP2();
    int i3 = t.getIndP3();

    Point p1 = points[i1];
    Point p2 = points[i2];
    Point p3 = points[i3];

    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLES);
    glVertex3f(p1.getX(), p1.getY(), p1.getZ());
    glVertex3f(p2.getX(), p2.getY(), p2.getZ());
    glVertex3f(p3.getX(), p3.getY(), p3.getZ());
    glEnd();
}

/**
 * Draw a figure, given the vertices and all the triangles
 * @param triangles list of triangles containing 3 indexes, specifying the positions of the triangle vertices in the points vector
 * @param points the set of all the points in the figure
 * @param red red color setting
 * @param green green color setting
 * @param blue blue color setting
 */
void drawFigure(vector<Triangle> *triangles, vector<Point> *points, float red, float green, float blue){
    int size = triangles->size();
    for(int i=0; i < size; i++){
        drawTriangle((*triangles)[i], red, green, blue, (*points));
    }
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
     
    // put the geometric transformations here
	
    // put the drawing instructions here

    //g.drawGroup(1,1,1);
    /*Point colors {1,1,0};
    Point center {50,50,50};
    drawEllipticalOrbit(center, 20, 50, colors, angle);
    angle += 0.1;*/
	// End of frame
    //glutWireTeapot(5);
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

void readXML(char* filePath){
    XMLDocument *doc = new XMLDocument();
    doc->LoadFile(filePath);
    XMLNode* world = doc->FirstChildElement("world");
    float fov, far, near;
    Point* cameraPosition = NULL, *cameraLookAt = NULL, *cameraUpVector = NULL;
  
    if (world) {
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
                cameraPosition = new Point(stof(position->Attribute("x")), stof(position->Attribute("y")), stof(position->Attribute("z")));
            }

            /* camera lookAt */
            XMLElement* lookAt = cameraElem->FirstChildElement("lookAt");
            if (lookAt) {
                cameraLookAt = new Point (stof(lookAt->Attribute("x")), stof(lookAt->Attribute("y")), stof(lookAt->Attribute("z")));
            }

            /* camera up */
            XMLElement* up = cameraElem->FirstChildElement("up");
            if (up) {
                cameraUpVector = new Point (stof(up->Attribute("x")), stof(up->Attribute("y")), stof(up->Attribute("z")));
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
}

int main(int argc, char **argv) {
    if (argc == 2) {

        readXML(argv[1]);

        // init GLUT and the window
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
        glutInitWindowPosition(100, 100);
        glutInitWindowSize(width, height);
        glutCreateWindow("ProjetoCG");

        // Required callback registry 
        glutIdleFunc(renderScene);
        glutDisplayFunc(renderScene);
        glutReshapeFunc(changeSize);


        // put here the registration of the keyboard callbacks
        glutSpecialFunc(processSpecialKeys);
        glutKeyboardFunc(keyboard_events);
        //glutPassiveMotionFunc(mouse_function);

        //  OpenGL settings
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        // enter GLUT's main cycle
        glutMainLoop();

    }
    else {
        cout << "Número incorreto de argumentos." << endl;
    }
	
	return 1;
}
