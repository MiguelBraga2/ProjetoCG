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
#include <string.h>
#include "../libraries/rapidxml-1.13/rapidxml_utils.hpp"
#include "../shared/triangle.hpp"
#include "../shared/point.hpp"
#include "../shared/IO.hpp"


float cameraPositionX, cameraPositionY, cameraPositionZ;
float cameraLookAtX, cameraLookAtY, cameraLookAtZ;
float cameraUpX, cameraUpY, cameraUpZ;
float pFov, pNear, pFar;
float width, height;
float rotationAlpha=0, rotationBeta=0;
float lookDirX, lookDirY, lookDirZ;
vector<char*> primitives{};
vector<vector<Point>*> points {};
vector<vector<Triangle>> figures {};

using namespace rapidxml;
using namespace std;

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
void drawFigure(vector<Triangle> triangles, vector<Point> points, float red, float green, float blue){
    for(int i=0; i<triangles.size(); i++){
        Triangle t = triangles[i];
        drawTriangle(t, red, green, blue, points);
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
	gluPerspective(pFov ,ratio, pNear ,pFar);
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(cameraPositionX,cameraPositionY,cameraPositionZ,
              cameraLookAtX,cameraLookAtY,cameraLookAtZ,
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
    
    for (int i=0; i<figures.size(); i++){
        drawFigure(figures [i], *(points[i]), 1,1,1);
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

void special_keyboard(int key_code, int x, int y){
    if (key_code == GLUT_KEY_LEFT){
        rotationAlpha -= M_PI/30;
    }
    else if (key_code == GLUT_KEY_RIGHT){
        rotationAlpha += M_PI/30;
    }
    else if (key_code == GLUT_KEY_UP){
        rotationBeta += M_PI/30;
    }
    else if (key_code == GLUT_KEY_DOWN){
        rotationBeta -= M_PI/30;
    }

    updateCamera();
}

void readXML(char* filePath){
    file<> xmlFile(filePath);
    // Create & parse document
    xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    // Get root node
    xml_node<> *world = doc.first_node("world");
    xml_node<> *window = world->first_node();
    width = stof(window->first_attribute()->value());
    height = stof(window->last_attribute()->value());
    xml_node<> *camera = window->next_sibling();
    xml_node<> *position = camera->first_node();
    xml_attribute<>* x = position->first_attribute();
    xml_attribute<>* y = x->next_attribute();
    xml_attribute<>* z = y->next_attribute();
    cameraPositionX = stof(x->value());
    cameraPositionY = stof(y->value());
    cameraPositionZ = stof(z->value());
    xml_node<> *lookAt = position->next_sibling();
    x = lookAt->first_attribute();
    y = x->next_attribute();
    z = y->next_attribute();
    cameraLookAtX = stof(x->value());
    cameraLookAtY = stof(y->value());
    cameraLookAtZ = stof(z->value());
    xml_node<> *up = lookAt->next_sibling();
    x = up->first_attribute();
    y = x->next_attribute();
    z = y->next_attribute();
    cameraUpX = stof(x->value());
    cameraUpY = stof(y->value());
    cameraUpZ = stof(z->value());
    xml_node<> *projection = up->next_sibling();
    xml_attribute<>* fov = projection->first_attribute();
    xml_attribute<>* near = fov->next_attribute();
    xml_attribute<>* far = near->next_attribute();
    pFov = stof(fov->value());
    pNear = stof(near->value());
    pFar = stof(far->value());
    xml_node<> *group = world->last_node();
    xml_node<> *models = group->first_node();
    for (xml_node<>* model = models->first_node();
        model; model = model->next_sibling()) {
        xml_attribute<>* file = model->first_attribute();
        char* str = (char *) malloc(sizeof(char)*50);
        strcpy(str, file->value());
        primitives.push_back(str);
    }
}

int main(int argc, char **argv) {
    if (argc == 2)
    {
        readXML(argv[1]);
        for (int i = 0; i < primitives.size(); i++) {
            figures.emplace_back(vector<Triangle>{});
            points.emplace_back(reader(primitives[i], &figures[i]));
        }

        // init GLUT and the window
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
        glutInitWindowPosition(100, 100);
        glutInitWindowSize(width, height);
        glutCreateWindow("ProjetoCG");
        glPolygonMode(GL_FRONT, GL_LINE);

        // Required callback registry 
        glutDisplayFunc(renderScene);
        glutReshapeFunc(changeSize);


        // put here the registration of the keyboard callbacks
        glutSpecialFunc(special_keyboard);
        glutKeyboardFunc(keyboard_events);
        //glutPassiveMotionFunc(mouse_function);

    //  OpenGL settings
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        // enter GLUT's main cycle
        glutMainLoop();
    }
    else 
    {
        cout << "Número de argumentos incorretos" << endl;
    }
	
	return 1;
}
