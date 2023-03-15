#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <list>
#include <iostream>
#include <string>
#include "../libraries/tinyxml2.h"
#include "../shared/triangle.hpp"
#include "../shared/IO.hpp"
#include "camera.hpp"

float width, height;
Camera* camera;
vector<vector<Point> *> *points;
vector<vector<Triangle> *> *figures;

float startAngle1 = 0, startAngle2 = 0.4, startAngleLua = 0;

using namespace tinyxml2;
using namespace std;

// Radians
void drawOrbit(float radius, float alpha, float red, float green, float blue, float equation, float radiusLua, float alphaLua, float redLua, float greenLua, float blueLua, float equationLua){
    float posX = radius * cos(alpha);
    float posZ = radius * sin(alpha);
    glPushMatrix();
    glTranslatef(posX, equation*posX, posZ);
    glColor3f(red,green,blue);
    glutSolidSphere(3, 100, 100);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(posX, equation*posX, posZ);
    float luaX = radiusLua * cos(alphaLua);
    float luaZ = radiusLua * sin(alphaLua);
    glTranslatef(luaX, equationLua*luaX, luaZ);
    glColor3f(redLua, greenLua, blueLua);
    glutSolidSphere(1, 100, 100);
    glPopMatrix();

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
	gluLookAt(camera->getPosition().getX(),camera->getPosition().getY(),camera->getPosition().getZ(),
              camera->getLookAtPosition().getX(),camera->getLookAtPosition().getY(),camera->getLookAtPosition().getZ(),
			  camera->getUpVector().getX(),camera->getUpVector().getY(),camera->getUpVector().getZ());

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

    int size = figures->size();
    for (int i=0; i< size; i++){
        drawFigure((*figures)[i], (*points)[i], 1,1,1);
    }
    glColor3f(1, 1, 0);
    glutSolidSphere(4, 100, 100);
    drawOrbit(100, startAngle1, 1, 0, 0, 1, 15, startAngleLua, 1,1,1, -2);
    drawOrbit(60, startAngle2, 0, 1, 0, 2, 20, startAngleLua, 1,1,1, -1);
    startAngle1 += 0.05;
    startAngle2 += 0.1;
    startAngleLua += 0.3;
	// End of frame
	glutSwapBuffers();
}

// write function to process keyboard events

void keyboard_events(unsigned char key, int x, int y) {
    if (key == 'w'){
        //camera->moveForwards();
    }
    else if (key == 's'){
        //camera->moveBackwards();
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

vector<string>* readXML(char* filePath){
    vector<string>* primitives = new vector<string>();
    XMLDocument *doc = new XMLDocument();
    XMLError result = doc->LoadFile(filePath);
    XMLNode* world = doc->FirstChildElement("world");
    float fov, far, near;
    Point* cameraPosition, *cameraLookAt, *cameraUpVector;
  
    if (world) {
        // window
        XMLElement* window = world->FirstChildElement("window");
        if (window) {
            width =  stof(window->Attribute("width"));
            height = stof(window->Attribute("height"));
        }
        
        // camera
        XMLElement* cameraAtt = world->FirstChildElement("camera");
        if (cameraAtt) {
            XMLElement* position = cameraAtt->FirstChildElement("position");
            if (position) {
                cameraPosition = new Point(stof(position->Attribute("x")),
                                             stof(position->Attribute("y")),
                                             stof(position->Attribute("z")));
            }
            XMLElement* lookAt = cameraAtt->FirstChildElement("lookAt");
            if (lookAt) {
                cameraLookAt = new Point (stof(lookAt->Attribute("x")), stof(lookAt->Attribute("y")), stof(lookAt->Attribute("z")));
            }
            XMLElement* up = cameraAtt->FirstChildElement("up");
            if (up) {
                cameraUpVector = new Point (stof(up->Attribute("x")), stof(up->Attribute("y")), stof(up->Attribute("z")));
            }
            XMLElement* projection = cameraAtt->FirstChildElement("projection");
            if (projection) {
                fov = stof(projection->Attribute("fov"));
                near = stof(projection->Attribute("near"));
                far = stof(projection->Attribute("far"));
            }
            camera = new Camera(*cameraPosition, *cameraLookAt, *cameraUpVector, fov, near, far);
        }

        // group
        XMLElement* group = world->FirstChildElement("group");
        if (group) {
            XMLElement* models = group->FirstChildElement("models");
            if (models) {
                for (XMLElement* model = models->FirstChildElement("model"); model != NULL; model = model->NextSiblingElement("model")) {
                    primitives->emplace_back(model->Attribute("file"));
                }
            }
        }
    }

    delete doc;

    return primitives;
}

int main(int argc, char **argv) {
    if (argc == 2)
    {
        vector<string> *primitives = readXML(argv[1]);

        figures = new vector<vector<Triangle> *>();
        points = new vector<vector<Point> *>();
        
        int size = primitives->size();
        for (int i = 0; i < size; i++) {
            figures->emplace_back(new vector<Triangle>());
            points->emplace_back(reader((*primitives)[i], (*figures)[i]));
        }

        primitives->clear();
        delete primitives;

        // init GLUT and the window
        glutInit(&argc, argv);
        //cout << betaAngle*180/M_PI << endl;
        glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
        glutInitWindowPosition(100, 100);
        glutInitWindowSize(width, height);
        glutCreateWindow("ProjetoCG");
        //glPolygonMode(GL_FRONT, GL_LINE);

        // Required callback registry 
        glutIdleFunc(renderScene);
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

        size = figures->size();
        for (int i = 0; i < size; i++) {
            vector<Triangle>* fig = (*figures)[i];
            delete fig;
        }
        delete figures;

        size = points->size();
        for (int i = 0; i < size; i++) {
            vector<Point>* pts = (*points)[i];
            delete pts;
        }
        delete points;
    }
    else 
    {
        cout << "Número incorreto de argumentos" << endl;
    }
	
	return 1;
}
