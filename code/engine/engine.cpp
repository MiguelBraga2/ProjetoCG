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
#include <GL/freeglut.h>
#include <GL/glx.h>
#endif

#include "../libraries/tinyxml2.h"
#include "camera.hpp"
#include "group.hpp"


using namespace tinyxml2;
using namespace std;

float width, height;
Camera* camera;
Group* group; // Outer collection of transformations, models and subgroups

// For FPS count
int timebase;
float frames;

int startX, startY, tracking = 0;

bool axis = false; // Is axis shown
int polygonMode = 0; // 0 - GL_FILL, 1 - GL_LINE, 2 - GL_POINT

vector<Point> teleports;

/**
 * Callback called when the window is resized
 * @param w width of the window
 * @param h height of the window
 */
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
 * Function to display the scene elements:
 * Calls the drawGroup function from the group class
 */
void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2i(0, 1);

    glColor3f(0, 0, 1.0f);
    const unsigned char* s = camera->toString();
    glutBitmapString(GLUT_BITMAP_HELVETICA_10, s);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    camera->placeCamera();

    if (axis){
        // put axis drawing in here
        glBegin(GL_LINES);

        // X-axis in red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-100.0f, 0.0f, 0.0f);
        glVertex3f( 100.0f, 0.0f, 0.0f);

        // Y Axis in Green
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f,-100.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);

        // Z Axis in Blue
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f,-100.0f);
        glVertex3f(0.0f, 0.0f, 100.0f);
        glEnd();
    }


    glColor3f(1.0f, 1.0f, 1.0f);
     
    // transformation and drawing instructions here
    group->drawGroup();

    frames++;
    int time = glutGet(GLUT_ELAPSED_TIME);
    int fps;
    if(time-timebase > 1000){
        fps = frames * 1000.0/(time-timebase);
        timebase = time;
        frames = 0;
        char* s = (char*)malloc(4);
        std::sprintf(s, "FPS: %d", fps);
        glutSetWindowTitle(s);
    }
    glPolygonMode(GL_FRONT, polygonMode);
    // End of frame
    glutSwapBuffers();
}

/**
 * Given the id of the menu, controls the changes to make
 * @param id id of the menu
 */
void menu(int id)
{
    switch(id)
    {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            if (axis == false) axis = true;
            else if (axis == true) axis = false;
            break;
        case 11:
            polygonMode = GL_FILL;
            break;
        case 12:
            polygonMode = GL_LINE;
            break;
        case 13:
            polygonMode = GL_POINT;
            break;
        default:
            camera->setPosition(teleports.at(id-14));
            break;
    }
}

/**
 * Creates a menu with options of:
 * - toggling axis
 * - travelling to planet locations
 */
void createMenu(void){
    int submenu, submenu2, submenu3;
    submenu2 = glutCreateMenu(menu);
    glutAddMenuEntry("GL_FILL", 11);
    glutAddMenuEntry("GL_LINE", 12);
    glutAddMenuEntry("GL_POINT", 13);
    submenu3 = glutCreateMenu(menu);
    for(int i=0; i<teleports.size(); i++){
        char label[20];
        sprintf(label, "Objeto %d", i+1);
        glutAddMenuEntry(label, 14+i);
    }
    glutCreateMenu(menu);
    glutAddSubMenu("Travel To", submenu3);
    glutAddSubMenu("Change polygon mode", submenu2);
    glutAddMenuEntry("Add axes", 10);


    glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

/**
 * Processing callback to handle mouse events
 * @param button
 * @param state
 * @param xx
 * @param yy
 */
void processMouseButtons(int button, int state, int xx, int yy) {
    if (state == GLUT_DOWN)  {
        startX = xx;
        startY = yy;
        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
        else tracking = 0;
    }
    else if (state == GLUT_UP) {
        camera->updateMouseAngles(tracking, (xx - startX), (yy - startY));
        tracking = 0;
    }
}

/**
 * To be called when the mouse moves
 * @param xx horizontal position of the mouse
 * @param yy vertical position of the mouse
 */
void processMouseMotion(int xx, int yy) {
    int deltaX = xx - startX;
    int deltaY = yy - startY;
    camera->processMouseMotion(tracking, deltaX, deltaY);
}

/**
 * function to process keyboard events (only for non-special keys)
 * @param key key pressed
 * @param x x coordinate of mouse in the screen
 * @param y y coordinate of mouse in the screen
 */
void keyboard_events(unsigned char key, int x, int y) {
    if (key == 'w'){
        camera->moveForwards();
    }
    else if (key == 's'){
        camera->moveBackwards();
    }
    else if (key == 'a'){
        camera->moveLeft();
    }
    else if (key == 'd'){
        camera->moveRight();
    }
    else if (key == '1'){
        camera->decrementIncrement();
    }
    else if (key == '2'){
        camera->incrementIncrement();
    }
    else if (key == 'm'){
        camera->changeMode();
    }

    glutPostRedisplay();
}

/**
 * function to process keyboard events (for special keys)
 * @param key key pressed
 * @param x x coordinate of mouse in the screen
 * @param y y coordinate of mouse in the screen
 */
void processSpecialKeys(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            camera->incrementAlfa(); break; // horizontal rotation

        case GLUT_KEY_LEFT:
            camera->decrementAlfa(); break; // horizontal rotation

        case GLUT_KEY_UP:
            camera->incrementBeta(); break; // vertical rotation

        case GLUT_KEY_DOWN:
            camera->decrementBeta(); break; // horizontal rotation

        case GLUT_KEY_PAGE_DOWN:
            camera->decrementRadius(); break;

        case GLUT_KEY_PAGE_UP:
            camera->incrementRadius(); break;

        case GLUT_KEY_HOME:
            camera->moveUp(); break;

        case GLUT_KEY_END:
            camera->moveDown(); break;
    }

    glutPostRedisplay();
}

/**
 * Parsing of the XML with the informations for the camera settings and groups
 * @param filePath the PATH of the XML
 * @return an error code (0 - ok, > 0 - something has gone wrong)
 */
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
        vector<Transformation> transf;
        group->initializeTeleporter(&transf, &teleports);

        for(Point p: teleports){
            cout << p.toString() << endl;
        }
    }

    delete doc;
    
    return (int) error;
}

// Main function
int main(int argc, char **argv) {
    if (argc == 2) {

        int error=readXML(argv[1]);

        if (!error) {

            // init GLUT and the window
            glutInit(&argc, argv);
            glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
            glutInitWindowPosition(100, 100);
            glutInitWindowSize(width, height);
            glutCreateWindow("ProjetoCG");
            createMenu();

            timebase = glutGet(GLUT_ELAPSED_TIME);

            // Required callback registry 
            glutIdleFunc(renderScene);
            glutDisplayFunc(renderScene);
            glutReshapeFunc(changeSize);


            // put here the registration of the keyboard callbacks
            glutSpecialFunc(processSpecialKeys);
            glutKeyboardFunc(keyboard_events);
            glutMouseFunc(processMouseButtons);
            glutMotionFunc(processMouseMotion);
            //glutPassiveMotionFunc(mouse_function);

            // Required for VBOs 
            glewInit();
            glEnableClientState(GL_VERTEX_ARRAY);

            group->prepareBuffers();

            // 	OpenGL settings
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT, polygonMode);

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
