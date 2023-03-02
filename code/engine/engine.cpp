#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <math.h>
#include "libraries/rapidxml-1.13/rapidxml_utils.hpp"

float cameraPositionX, cameraPositionY, cameraPositionZ;
float cameraLookAtX, cameraLookAtY, cameraLookAtZ;
float cameraUpX, cameraUpY, cameraUpZ;
float pFov, pNear, pFar;
float width, height;
float rotationAlpha=0, rotationBeta=0;
float lookDirX, lookDirY, lookDirZ;

using namespace rapidxml;
using namespace std;

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

void drawSphere(float radius, int numSlices, int numStacks){
    float alpha = M_PI/numSlices; // Defines the position around the y axis
    float initialBeta = M_PI/numStacks; // Defines the height
    float beta = -M_PI/2 + (numStacks-1)* initialBeta;

    glColor3f(1.0f, 1.0f, 0.0f);
    for (int i=0; i<numSlices*2; i++){
        float nextAlpha = (i+1)*alpha;

        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, radius, 0.0f);
        glVertex3f(radius*cos(beta)*sin(i*alpha), radius*sin(beta), radius*cos(i*alpha)*cos(beta));
        glVertex3f(radius*cos(beta)*sin(nextAlpha), radius*sin(beta), radius*cos(nextAlpha)*cos(beta));
        glEnd();

    }

    for(int i=0; i<numStacks-2; i++){ // Draw each of the vertical divisions
        float currentBeta = -M_PI/2+(i+1)*initialBeta; // height
        float nextBeta = -M_PI/2+(i+2)*initialBeta; // height above


        for (int j=0; j<numSlices*2; j++){
            float currentAlpha = j*alpha;
            float nextAlpha = (j+1)*alpha;

            glBegin(GL_TRIANGLES);
            glVertex3f(radius*cos(nextBeta)*sin(currentAlpha), radius*sin(nextBeta), radius*cos(nextBeta)*cos(currentAlpha));
            glVertex3f(radius*cos(currentBeta)*sin(currentAlpha), radius*sin(currentBeta), radius*cos(currentBeta)*cos(currentAlpha));
            glVertex3f(radius*cos(currentBeta)*sin(nextAlpha), radius*sin(currentBeta), radius*cos(currentBeta)*cos(nextAlpha));
            glEnd();

            glBegin(GL_TRIANGLES);
            glVertex3f(radius*cos(nextBeta)*sin(nextAlpha), radius*sin(nextBeta), radius*cos(nextBeta)*cos(nextAlpha));
            glVertex3f(radius*cos(nextBeta)*sin(currentAlpha), radius*sin(nextBeta), radius*cos(nextBeta)*cos(currentAlpha));
            glVertex3f(radius*cos(currentBeta)*sin(nextAlpha), radius*sin(currentBeta), radius*cos(currentBeta)*cos(nextAlpha));
            glEnd();

        }
    }

    beta = -M_PI/2+initialBeta;

    for (int i=0; i<numSlices*2; i++){
        float nextAlpha = (i+1)*alpha;

        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, -radius, 0.0f);
        glVertex3f(radius*cos(beta)*sin(nextAlpha), radius*sin(beta), radius*cos(nextAlpha)*cos(beta));
        glVertex3f(radius*cos(beta)*sin(i*alpha), radius*sin(beta), radius*cos(i*alpha)*cos(beta));

        glEnd();

    }
}

void renderScene(void) {
    glPolygonMode(GL_FRONT, GL_LINE);
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(cameraPositionX,cameraPositionY,cameraPositionZ,
              cameraPositionX + cameraLookAtX,cameraPositionY + cameraLookAtY,cameraPositionZ + cameraLookAtZ,
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
	

// put pyramid drawing instructions here
	//glutWireSphere(1, 10, 10);
    drawSphere(2, 18,22);
	// End of frame
	glutSwapBuffers();
}

void crossProduct(float vectAX, float vectAY, float vectAZ, float vectBX, float vectBY, float vectBZ, float cross_P[])
{
    cross_P[0] = vectAY * vectBZ - vectAZ * vectBY;
    cross_P[1] = vectAZ * vectBX - vectAX * vectBZ;
    cross_P[2] = vectAX * vectBY - vectAY * vectBX;
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
    cout << "Width: " << window->first_attribute()->value() << endl;
    cout << "Height: " << window->last_attribute()->value() << endl;
    width = atof(window->first_attribute()->value());
    height = atof(window->last_attribute()->value());
    xml_node<> *camera = window->next_sibling();
    xml_node<> *position = camera->first_node();
    xml_attribute<>* x = position->first_attribute();
    xml_attribute<>* y = x->next_attribute();
    xml_attribute<>* z = y->next_attribute();
    cameraPositionX = atof(x->value());
    cameraPositionY = atof(y->value());
    cameraPositionZ = atof(z->value());
    cout << "Pos X: " << x->value() << endl;
    cout << "Pos Y: " << y->value() << endl;
    cout << "Pos Z: " << z->value() << endl;
    xml_node<> *lookAt = position->next_sibling();
    x = lookAt->first_attribute();
    y = x->next_attribute();
    z = y->next_attribute();
    cameraLookAtX = atof(x->value());
    cameraLookAtY = atof(y->value());
    cameraLookAtZ = atof(z->value());
    cout << "Look at X: " << x->value() << endl;
    cout << "Look at Y: " << y->value() << endl;
    cout << "Look at Z: " << z->value() << endl;
    xml_node<> *up = lookAt->next_sibling();
    x = up->first_attribute();
    y = x->next_attribute();
    z = y->next_attribute();
    cameraUpX = atof(x->value());
    cameraUpY = atof(y->value());
    cameraUpZ = atof(z->value());
    cout << "Up X: " << x->value() << endl;
    cout << "Up Y: " << y->value() << endl;
    cout << "Up Z: " << z->value() << endl;
    xml_node<> *projection = up->next_sibling();
    xml_attribute<>* fov = projection->first_attribute();
    xml_attribute<>* near = fov->next_attribute();
    xml_attribute<>* far = near->next_attribute();
    pFov = atof(fov->value());
    pNear = atof(near->value());
    pFar = atof(far->value());
    cout << "FOV: " << fov->value() << endl;
    cout << "Near: " << near->value() << endl;
    cout << "Far: " << far->value() << endl;
    xml_node<> *group = world->last_node();
}

int main(int argc, char **argv) {
    readXML("../demo.xml");
	// Parser XML
	// Configurar câmara e etc
	// Ler os ficheiros .3d (que devem estar com a ordem correta)
	// Mostrar os triângulos

	
// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(width,height);
	glutCreateWindow("ProjetoCG");
		
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
	
	return 1;
}
