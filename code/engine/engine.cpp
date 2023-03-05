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
#include "../libraries/rapidxml-1.13/rapidxml_utils.hpp"
#include "../shared/triangle.hpp"
#include "../shared/point.hpp"
#include "../shared/Parser.hpp"


float cameraPositionX, cameraPositionY, cameraPositionZ;
float cameraLookAtX, cameraLookAtY, cameraLookAtZ;
float cameraUpX, cameraUpY, cameraUpZ;
float pFov, pNear, pFar;
float width, height;
float rotationAlpha=0, rotationBeta=0;
float lookDirX, lookDirY, lookDirZ;
vector<char*> primitives{};

using namespace rapidxml;
using namespace std;

void crossProduct(float vectAX, float vectAY, float vectAZ, float vectBX, float vectBY, float vectBZ, float cross_P[])
{
    cross_P[0] = vectAY * vectBZ - vectAZ * vectBY;
    cross_P[1] = vectAZ * vectBX - vectAX * vectBZ;
    cross_P[2] = vectAX * vectBY - vectAY * vectBX;
}

void drawTriangle(Point normal_vector, Triangle t, float red, float green, float blue, vector<Point> points){
    int i1 = t.getIndP1();
    int i2 = t.getIndP2();
    int i3 = t.getIndP3();

    Point p1 = points[i1];
    Point p2 = points[i2];
    Point p3 = points[i3];

    if (normal_vector.getX() == 0 && normal_vector.getY() == 0 && normal_vector.getZ() == 0){
        glBegin(GL_TRIANGLES);
        glVertex3f(p1.getX(), p1.getY(), p1.getZ());
        glVertex3f(p2.getX(), p2.getY(), p2.getZ());
        glVertex3f(p3.getX(), p3.getY(), p3.getZ());
        glEnd();
        return;
    }

    Point v1 = Point (p2.getX()-p1.getX(), p2.getY()-p1.getY(), p2.getZ() - p1.getZ());
    Point v2 = Point (p3.getX()-p2.getX(), p3.getY()-p2.getY(), p3.getZ() - p2.getZ());

    float cross_P[3];
    crossProduct(v1.getX(), v1.getY(), v1.getZ(), v2.getX(), v2.getY(), v2.getZ(), cross_P);

    Point v3 (cross_P[0], cross_P[1], cross_P[2]);
    float norma = sqrt(pow(cross_P[0], 2) + pow(cross_P[1], 2) + pow(cross_P[2], 2));

    v3.setX(v3.getX()/norma);
    v3.setY(v3.getY()/norma);
    v3.setZ(v3.getZ()/norma);

    // Iguais
    if (abs(v3.getX() - normal_vector.getX()) < 0.1 && abs(v3.getY() - normal_vector.getY()) < 0.1 && abs(v3.getZ() -normal_vector.getZ()) < 0.1) {

        glBegin(GL_TRIANGLES);
        glVertex3f(p1.getX(), p1.getY(), p1.getZ());
        glVertex3f(p2.getX(), p2.getY(), p2.getZ());
        glVertex3f(p3.getX(), p3.getY(), p3.getZ());
        glEnd();
    } // Simétricos
    else if (abs(v3.getX() + normal_vector.getX()) < 0.1 && abs(v3.getY() + normal_vector.getY()) < 0.1 && abs(v3.getZ() + normal_vector.getZ()) < 0.1) {

        glBegin(GL_TRIANGLES);
        glVertex3f(p1.getX(), p1.getY(), p1.getZ());
        glVertex3f(p3.getX(), p3.getY(), p3.getZ());
        glVertex3f(p2.getX(), p2.getY(), p2.getZ());
        glEnd();
    }

}


void drawFigure(vector<Point> normals, vector<Triangle> triangles, vector<Point> points, vector<int> normal_indexes, float red, float green, float blue){
    int index = 0;

    for(int i=0; i<triangles.size(); i++){
        Triangle t = triangles[i];

        if (normal_indexes.size() > 0){
            auto itN = normal_indexes.begin();
            advance(itN, index);

            auto itN2 = normals.begin();
            advance(itN2, *itN-1);

            Point normal (itN2->getX(), itN2->getY(), itN2->getZ());
            drawTriangle(normal, t, 1,1,0, points);
        }
        else {
            Point normal(0,0,0);
            drawTriangle(normal, t, 1,1,0, points);
        }

        index++;

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
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();
     
    // put the geometric transformations here
	
    // put the drawing instructions here
    
    //drawFigure(triangles, 1, 1, 0);
    for (int i = 0; i < primitives.size(); i++) {
        vector<Triangle> triangles{};
        vector<Point> normals{};
        vector<int> normal_indexes{};
        vector<Point> points = reader(primitives[i], &triangles, &normals, &normal_indexes);
        drawFigure(normals, triangles, points, normal_indexes, 1, 1, 0);
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


/*
void mouse_function(int x, int y){
    // width height
    float midY = y/2;
    float midX = x/2;

    float addAlpha;
    float addBeta;

    if (x > midX){
        float step = midX/30;
        float steps = ((x-midX)/step);
        addAlpha = steps * M_PI/30;
        if (y > midY){
            float step = midY/30;
            float steps = ((y-midY)/step);
            addBeta = steps * M_PI/30; // ângulo
        }
        else {
            float step = midY/30;
            float steps = (y/step);
            addBeta = steps * M_PI/30; // ângulo
        }
    }
    else {
        float step = midX/30;
        float steps = ((x-midX)/step);
        addAlpha = -steps * M_PI/30;
        if (y > midY){
            float step = midY/30;
            float steps = ((y-midY)/step);
            addBeta = steps * M_PI/30; // ângulo
        }
        else {
            float step = midY/30;
            float steps = (y/step);
            addBeta = steps * M_PI/30; // ângulo
        }
    }

    rotationAlpha += addAlpha;
    rotationBeta += addBeta;

    updateCamera();

    glutPostRedisplay();
}*/

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
	// Parser XML
    if (argc == 2)
    {
        readXML(argv[1]);

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
