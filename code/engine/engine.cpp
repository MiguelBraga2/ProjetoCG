#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <map>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "../libraries/tinyxml2.h"
#include "camera.hpp"
#include "group.hpp"
#include "Transformations/translation.hpp"
#include "Transformations/rotation.hpp"
#include "creator.h"

using namespace std;

int width, height;
Camera* camera;
Group* globalGroup; // Outer collection of transformations, models and subgroups

// For FPS count
int timebase;
float frames;

bool axis = true; // Is axis shown
bool cameraInfo = false;
bool fixedMode = false;
string fixedLabel = "";
int polygonMode = GL_LINE;

// For each label, store the center and the radius
map<string, tuple<Point, float>> teleports;
vector<string> keys; // To make mapping from number to label easier

bool vboActive = true;


// MINECRAFT
bool isMinecraftActive = false;
Creator* minecraftCreator;

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
	float ratio = (float) w * 1.0f / (float) h;

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
 * Function to render text
 */
void renderText() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    // set projection so that coordinates match window pixels
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_DEPTH_TEST);

    glPushMatrix();
    glLoadIdentity();
    glRasterPos2d(0, 0); // text position in pixels

    if (cameraInfo)
        for (const unsigned char *c = camera->toString(); *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
        }

    if (camera->getMode() == 1) {
        glRasterPos2d(width/2 - 1, height/2 - 1); // text position in pixels
        for (const char *c = "+"; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
}

/**
 * Function to display the scene elements:
 * Calls the drawGroup function from the group class
 */
void renderScene() {
    glPolygonMode(GL_FRONT, polygonMode);
    if (!isMinecraftActive) {
        // clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set the camera
        glLoadIdentity();
        if (fixedLabel.empty())
            camera->placeCamera();

        if (fixedMode == true && !fixedLabel.empty()) {
            vector<Transformation *> transforms;
            teleports = globalGroup->initializeTps(&transforms);
            transforms.clear();
            camera->setLookAtPosition(get<0>(teleports[fixedLabel]));
            camera->setCameraRadius(get<1>(teleports[fixedLabel]));
            camera->placeCamera();
        }

        glColor3f(1.0f, 1.0f, 1.0f);

        globalGroup->drawGroup(vboActive);

        renderText();

        if (axis) {
            // put axis drawing in here
            glBegin(GL_LINES);

            // X-axis in red
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-100.0f, 0.0f, 0.0f);
            glVertex3f(100.0f, 0.0f, 0.0f);

            // Y Axis in Green
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, -100.0f, 0.0f);
            glVertex3f(0.0f, 100.0f, 0.0f);

            // Z Axis in Blue
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, -100.0f);
            glVertex3f(0.0f, 0.0f, 100.0f);
            glEnd();
        }

        frames++;
        int time = glutGet(GLUT_ELAPSED_TIME);
        int fps;

        if (time - timebase > 1000) {
            fps = frames * 1000.0 / (time - timebase);
            timebase = time;
            frames = 0;
            char s[15];
            sprintf(s, "FPS: %d", fps);
            glutSetWindowTitle(s);
        }
        // End of frame
        glutSwapBuffers();
    }
    else{
        minecraftCreator->render(height, width);

        frames++;
        int time = glutGet(GLUT_ELAPSED_TIME);
        int fps;

        if (time - timebase > 1000) {
            fps = frames * 1000.0 / (time - timebase);
            timebase = time;
            frames = 0;
            char s[15];
            sprintf(s, "FPS: %d", fps);
            glutSetWindowTitle(s);
        }
    }
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
            camera->changeMode(0); // Explorer
            break;
        case 4:
            camera->changeMode(1); // FPS
            break;
        case 5:
            camera->changeMode(2); // Mouse motion
            break;
        case 6:
            if (fixedMode == true) { fixedMode = false; fixedLabel = ""; }
            else if (fixedMode == false) fixedMode = true;
            break;
        case 7:
            break;
        case 8:
            if (vboActive == true) vboActive = false;
            else if (vboActive == false) vboActive = true;
            break;
        case 9:
            if (cameraInfo) cameraInfo = false;
            else if (!cameraInfo) cameraInfo = true;
            break;
        case 10:
            if (!axis) axis = true;
            else if (axis) axis = false;
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
        case 14:
            camera->setLookAtPosition(Point(0,0,0));
            camera->setCameraRadius(10000);
            camera->setAlpha(0.785); // ~ 45º
            camera->setBeta(0.785); // ~ 45º
            camera->spherical2Cartesian();
            camera->setMode(0);
            break;
        default:
            vector<Transformation*> transforms;
            if (fixedMode == true){
                fixedLabel = keys[id-15];
            }
            else{
                teleports = globalGroup->initializeTps(&transforms);
                transforms.clear();
                camera->setLookAtPosition(get<0>(teleports[keys[id-15]]));
                camera->setCameraRadius(get<1>(teleports[keys[id-15]]));
                camera->setAlpha(0.785); // ~ 45º
                camera->setBeta(0.785); // ~ 45º
                camera->spherical2Cartesian();
                camera->setMode(0);
            }
            break;
    }
    glutPostRedisplay();
}

/**
 * Creates a menu with options of:
 * - toggling axis
 * - travelling to planet locations
 */
void createMenu(void){
    int submenu2, submenu3, submenu4;

    submenu2 = glutCreateMenu(menu);
    glutAddMenuEntry("GL_FILL", 11);
    glutAddMenuEntry("GL_LINE", 12);
    glutAddMenuEntry("GL_POINT", 13);

    submenu3 = glutCreateMenu(menu);
    int i=0;
    glutAddMenuEntry("Origin", 14);
    for (string label:keys) {
        const char* l = label.c_str();
        glutAddMenuEntry(l, 15+i);
        i++;
    }

    submenu4 = glutCreateMenu(menu);
    glutAddMenuEntry("Explorer mode", 3);
    glutAddMenuEntry("FPS mode", 4);
    glutAddMenuEntry("Mouse mode", 5);

    glutCreateMenu(menu);
    glutAddSubMenu("Travel To", submenu3);
    glutAddSubMenu("Change polygon mode", submenu2);
    glutAddMenuEntry("Change Teleport Mode", 6);
    glutAddSubMenu("Camera mode", submenu4);

    glutAddMenuEntry("Add axes", 10);
    glutAddMenuEntry("Show camera info", 9);
    glutAddMenuEntry("Toggle vbo mode", 8);

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
    if (isMinecraftActive == true){
        minecraftCreator->processMouseButtons(button, state, xx, yy);
    } else {
        camera->updateMouseAngles(button, state, xx, yy);
    }
}

/**
 * To be called when the mouse moves
 * @param xx horizontal position of the mouse
 * @param yy vertical position of the mouse
 */
void processMouseMotion(int xx, int yy) {
    camera->processMouseMotion(xx, yy);
}

/**
 * function to process keyboard events (only for non-special keys)
 * @param key key pressed
 * @param x x coordinate of mouse in the screen
 * @param y y coordinate of mouse in the screen
 */
void keyboard_events(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W'){
        camera->moveForwards();
    }
    else if (key == 's' || key == 'S'){
        camera->moveBackwards();
    }
    else if (key == 'a' || key == 'A'){
        camera->moveLeft();
    }
    else if (key == 'd' || key == 'D'){
        camera->moveRight();
    }
    else if (key == '1'){
        camera->decrementIncrement();
    }
    else if (key == '2'){
        camera->incrementIncrement();
    }
    else if (key == 'm'){
        if (isMinecraftActive == true) {
            isMinecraftActive = false;
            minecraftCreator->~Creator();
        }
        else {
            isMinecraftActive = true;
            camera->changeMode(1);
            minecraftCreator = new Creator(camera);
        }
    }
    else if (key == 'v'){
        minecraftCreator->changeBlockColor();
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
int readXML(char* filePath, vector<string>* keys){
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(filePath);
    tinyxml2::XMLNode* world = doc.FirstChildElement("world");

    float fov = 0, farV = 0, nearV = 0;
    Point cameraPosition, cameraLookAt, cameraUpVector;
  
    if (!error && world) {
        /* window */
        tinyxml2::XMLElement* windowElem = world->FirstChildElement("window");
        if (windowElem) {
            width =  stoi(windowElem->Attribute("width"));
            height = stoi(windowElem->Attribute("height"));
        }
        
        /* camera */
        tinyxml2::XMLElement* cameraElem = world->FirstChildElement("camera");
        if (cameraElem) {
            
            /* camera position */
            tinyxml2::XMLElement* position = cameraElem->FirstChildElement("position");
            if (position) {
                cameraPosition.setX(stof(position->Attribute("x")));
                cameraPosition.setY(stof(position->Attribute("y")));
                cameraPosition.setZ(stof(position->Attribute("z")));
            }

            /* camera lookAt */
            tinyxml2::XMLElement* lookAt = cameraElem->FirstChildElement("lookAt");
            if (lookAt) {
                cameraLookAt.setX(stof(lookAt->Attribute("x")));
                cameraLookAt.setY(stof(lookAt->Attribute("y")));
                cameraLookAt.setZ(stof(lookAt->Attribute("z")));
            }

            /* camera up */
            tinyxml2::XMLElement* up = cameraElem->FirstChildElement("up");
            if (up) {
                cameraUpVector.setX(stof(up->Attribute("x")));
                cameraUpVector.setY(stof(up->Attribute("y")));
                cameraUpVector.setZ(stof(up->Attribute("z")));
            }
            
            /* camera projection */
            tinyxml2::XMLElement* projection = cameraElem->FirstChildElement("projection");
            if (projection) {
                fov = stof(projection->Attribute("fov"));
                nearV = stof(projection->Attribute("near"));
                farV = stof(projection->Attribute("far"));
            }
          
            camera = new Camera(cameraPosition, cameraLookAt, cameraUpVector, fov, nearV, farV);
        }

        globalGroup = new Group();
        globalGroup->readXML(world->FirstChildElement("group"), keys);
    }

    return (int) error;
}

// Main function
int main(int argc, char **argv) {
    if (argc == 2) {

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
        glutMouseFunc(processMouseButtons);
        glutMotionFunc(processMouseMotion);

        // Required for VBOs
        glewInit();
        glEnableClientState(GL_VERTEX_ARRAY);

        if (isMinecraftActive == false) {

            int error = readXML(argv[1], &keys);

            if (!error) {
                createMenu();

                // 	OpenGL settings
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_CULL_FACE);
                glPolygonMode(GL_FRONT, polygonMode);

                // enter GLUT's main cycle
                glutMainLoop();

                globalGroup->freeGroup();
                delete globalGroup;
            } else {
                cout << "Error" << endl;
            }
        }
    }
    else {
        cout << "Número incorreto de argumentos." << endl;
    }
	
	return 1;
}
