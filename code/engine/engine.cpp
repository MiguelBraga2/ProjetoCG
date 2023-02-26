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
	

// put pyramid drawing instructions here
	glutWireSphere(1, 10, 10);

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events




using namespace rapidxml;
using namespace std;

int main(int argc, char **argv) {
	// Parser XML
	// Configurar câmara e etc
	// Ler os ficheiros .3d (que devem estar com a ordem correta)
	// Mostrar os triângulos
	
	file<> xmlFile("../demo.xml");
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



//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
