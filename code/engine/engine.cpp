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
#include "../shared/point.hpp"
#include "../shared/IO.hpp"

float alfa = 0.0f, betaAngle, cameraRadius;

float cameraPositionX, cameraPositionY, cameraPositionZ,
    cameraLookAtX, cameraLookAtY, cameraLookAtZ,
    cameraUpX, cameraUpY, cameraUpZ,
    fov, near, far,
    width, height,
    rotationAlpha=0, rotationBeta=0,
    lookDirX, lookDirY, lookDirZ;
vector<vector<Point> *> *points;
vector<vector<Triangle> *> *figures;
int cameraMode = 0; // 0 - Modo explorador, 1 - Modo FPS



using namespace tinyxml2;
using namespace std;

void spherical2Cartesian() {

    cameraPositionX = cameraRadius * cos(betaAngle) * sin(alfa);
    cameraPositionY = cameraRadius * sin(betaAngle);
    cameraPositionZ = cameraRadius * cos(betaAngle) * cos(alfa);
}

float calculateBeta(){
    // r * sin(beta) = y
    // sin(beta) = y/r
    // beta = arcsin(y/r)

    float acsin = asin(cameraPositionY/cameraRadius);

    return acsin;

}

float calculateAlpha(){
    float camPositionY = cameraLookAtY; // Projection in the same y-plane as the lookAtPoint
    float zSide = cameraPositionZ-cameraLookAtZ; // Length of the side of the triangle parallel to the z axis
    float xSide = cameraPositionX-cameraLookAtX; // Length of the side of the triangle parallel to the x axis
    float hip = sqrt(pow(cameraPositionX-cameraLookAtX, 2) + pow(cameraPositionY-cameraLookAtZ, 2)); // the hypotenuse

    float accos = acos(zSide/hip); // Angle of Beta (if in the 1st or 2nd quadrants)

    if (xSide < 0){ // Adjust if the angle is in the 3rd or 4th quadrants
        accos = M_PI - accos + M_PI;
    }

    cout << accos*180/M_PI << endl;
    return accos;
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

void drawTorus(float innerRadius, float outerRadius, float slices, float stacks){
    float alpha = 2*M_PI/slices; // Defines the position around the y axis
    float beta = 2*M_PI/stacks; // Defines the height
    vector<Triangle> figure {};
    float radius = (outerRadius-innerRadius)/2;
    float distanceToOrigin = innerRadius + radius;

    for(int i=0; i<slices; i++){ // Percorrer as slices
        float currentAlpha = i*alpha;
        for(int j=0; j<2*stacks; j++){ // Percorrer as stacks
            float currentBeta = -M_PI/2 + j*beta;
            float nextBeta = -M_PI/2 + (j+1)*beta;
            float nextAlpha = (i+1)*alpha;

            Point c(distanceToOrigin*sin(currentAlpha),0,distanceToOrigin*cos(currentAlpha));
            Point c2(distanceToOrigin*sin(nextAlpha),0,distanceToOrigin*cos(nextAlpha));

            Point p1 (radius*cos(currentBeta)*sin(currentAlpha),radius*sin(currentBeta), radius*cos(currentBeta)*cos(currentAlpha));
            Point p2 (radius*cos(nextBeta)*sin(currentAlpha),radius*sin(nextBeta), radius*cos(nextBeta)*cos(currentAlpha));
            Point p3 (radius*cos(nextBeta)*sin(nextAlpha),radius*sin(nextBeta), radius*cos(nextBeta)*cos(nextAlpha));
            Point p4 (radius*cos(currentBeta)*sin(nextAlpha),radius*sin(currentBeta), radius*cos(currentBeta)*cos(nextAlpha));

            Point p5(p1.getX() + c.getX(), p1.getY() + c.getY(), p1.getZ() + c.getZ());
            Point p6(p2.getX() + c.getX(), p2.getY() + c.getY(), p2.getZ() + c.getZ());
            Point p7(p3.getX() + c2.getX(), p3.getY() + c2.getY(), p3.getZ() + c2.getZ());
            Point p8(p4.getX() + c2.getX(), p4.getY() + c2.getY(), p4.getZ() + c2.getZ());

            glColor3f(1, 1, 0);

            glBegin(GL_TRIANGLES);
            glVertex3f(p7.getX(), p7.getY(), p7.getZ());
            glVertex3f(p6.getX(), p6.getY(), p6.getZ());
            glVertex3f(p5.getX(), p5.getY(), p5.getZ());
            glEnd();

            glBegin(GL_TRIANGLES);
            glVertex3f(p7.getX(), p7.getY(), p7.getZ());
            glVertex3f(p5.getX(), p5.getY(), p5.getZ());
            glVertex3f(p8.getX(), p8.getY(), p8.getZ());
            glEnd();
            // Interior do torus
            /*if (cos(currentBeta) > 0){

            } // Exterior do torus
            else {
                glBegin(GL_TRIANGLES);
                glVertex3f(p7.getX(), p7.getY(), p7.getZ());
                glVertex3f(p5.getX(), p5.getY(), p5.getZ());
                glVertex3f(p6.getX(), p6.getY(), p6.getZ());
                glEnd();

                glBegin(GL_TRIANGLES);
                glVertex3f(p7.getX(), p7.getY(), p7.getZ());
                glVertex3f(p8.getX(), p8.getY(), p8.getZ());
                glVertex3f(p5.getX(), p5.getY(), p5.getZ());
                glEnd();
            }*/
        }
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
	gluPerspective(fov ,ratio, near , far);
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(cameraPositionX,cameraPositionY,cameraPositionZ,
              cameraLookAtX/*+cameraPositionX*/,cameraLookAtY/*+cameraPositionY*/,cameraLookAtZ/*+cameraPositionZ*/,
			  cameraUpX,cameraUpY,cameraUpZ);

    // put axis drawing in here
    glBegin(GL_LINES);

    // X axis in red
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

	// End of frame
	glutSwapBuffers();
}

// write function to process keyboard events

void keyboard_events(unsigned char key, int x, int y) {
    if (key == 'w'){
        cameraPositionX += cameraLookAtX;
        cameraPositionY += cameraLookAtY;
        cameraPositionZ += cameraLookAtZ;
    }
    else if (key == 's'){
        cameraPositionX -= cameraLookAtX;
        cameraPositionY -= cameraLookAtY;
        cameraPositionZ -= cameraLookAtZ;
    }
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
    }

    glutPostRedisplay();
}

void updateCamera(){
    cameraLookAtX = cos(rotationBeta)*sin(rotationAlpha);
    cameraLookAtY = sin(rotationBeta);
    cameraLookAtZ = cos(rotationAlpha)*cos(rotationBeta);

    float lookAtLength = sqrt(pow(cameraLookAtX, 2) + pow(cameraLookAtY, 2) + pow(cameraLookAtZ, 2));
    cameraLookAtX = cameraLookAtX / lookAtLength;
    cameraLookAtY = cameraLookAtY / lookAtLength;
    cameraLookAtZ = cameraLookAtZ / lookAtLength;

    glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy) {

    switch (key) {

        case GLUT_KEY_RIGHT:
            alfa += 0.1; break;

        case GLUT_KEY_LEFT:
            alfa -= 0.1; break;

        case GLUT_KEY_UP:
            betaAngle += 0.1f;
            if (betaAngle > 1.5f)
                betaAngle = 1.5f;
            break;

        case GLUT_KEY_DOWN:
            betaAngle -= 0.1f;
            if (betaAngle < -1.5f)
                betaAngle = -1.5f;
            break;

        case GLUT_KEY_PAGE_DOWN: cameraRadius -= 0.1f;
            if (cameraRadius < 0.1f)
                cameraRadius = 0.1f;
            break;

        case GLUT_KEY_PAGE_UP: cameraRadius += 0.1f; break;
    }
    spherical2Cartesian();
    glutPostRedisplay();
}

vector<string>* readXML(char* filePath){
    vector<string>* primitives = new vector<string>();
    XMLDocument *doc = new XMLDocument();
    XMLError result = doc->LoadFile(filePath);
    XMLNode* world = doc->FirstChildElement("world");
  
    if (world) {
        // window
        XMLElement* window = world->FirstChildElement("window");
        if (window) {
            width =  stof(window->Attribute("width"));
            height = stof(window->Attribute("height"));
        }
        
        // camera
        XMLElement* camera = world->FirstChildElement("camera");
        if (camera) {
            XMLElement* position = camera->FirstChildElement("position");
            if (position) {
                cameraPositionX = stof(position->Attribute("x"));
                cameraPositionY = stof(position->Attribute("y"));
                cameraPositionZ = stof(position->Attribute("z"));
            }
            XMLElement* lookAt = camera->FirstChildElement("lookAt");
            if (lookAt) {
                cameraLookAtX = stof(lookAt->Attribute("x"));
                cameraLookAtY = stof(lookAt->Attribute("y"));
                cameraLookAtZ = stof(lookAt->Attribute("z"));
            }
            XMLElement* up = camera->FirstChildElement("up");
            if (up) {
                cameraUpX = stof(up->Attribute("x"));
                cameraUpY = stof(up->Attribute("y"));
                cameraUpZ = stof(up->Attribute("z"));
            }
            XMLElement* projection = camera->FirstChildElement("projection");
            if (projection) {
                fov = stof(projection->Attribute("fov"));
                near = stof(projection->Attribute("near"));
                far = stof(projection->Attribute("far"));
            }
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
        cameraRadius = sqrt(pow(cameraPositionX-cameraLookAtX, 2) + pow(cameraPositionY-cameraLookAtY, 2) + pow(cameraPositionZ-cameraLookAtZ, 2));
        alfa = calculateAlpha();
        betaAngle = calculateBeta();

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
        glPolygonMode(GL_FRONT, GL_LINE);

        // Required callback registry 
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
