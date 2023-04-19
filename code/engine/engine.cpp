#define MINIAUDIO_IMPLEMENTATION
#define MA_DEBUG_OUTPUT
#include "../libraries/miniaudio.h"

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
#include "Creator.h"


//using namespace tinyxml2;
using namespace std;


float width, height;
Camera* camera;
Group* group; // Outer collection of transformations, models and subgroups

// For FPS count
int timebase;
float frames;

float camX = 00, camY = 300, camZ = 400; //
int alpha = 0, myBeta = 45, r = 50; //
int startX, startY, tracking = 0;




bool axis = true; // Is axis shown
bool cameraInfo = true;
int polygonMode = GL_LINE;
int vboMode = 1; // 0 - noVBOs , 1 - VBOs

// For each label, store the center and the radius
map<string, tuple<Point, float>> teleports;
vector<string> keys; // To make mapping from number to label easier
vector<Group> cubes;

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

void displaySound(const char* file) {
    ma_result result;
    ma_engine engine;


    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.");
    }

    result = ma_engine_play_sound(&engine, file, NULL);

    std::cout << result << std::endl;

    printf("Press Enter to quit...");
    getchar();

    ma_engine_uninit(&engine);
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

    for (const unsigned char *c = camera->toString(); *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
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
void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
    camera->placeCamera();

    if (cameraInfo)
        renderText();

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
    group->drawGroup(vboMode);

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
            vboMode = 0;
            break;
        case 2:
            vboMode = 1;
            break;
        case 3:
            displaySound("../audio/turbinada.mp3");
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
            if (cameraInfo == true) cameraInfo = false;
            else if (cameraInfo == false) cameraInfo = true;
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
        case 14:
            camera->setLookAtPosition(Point(0,0,0));
            camera->setCameraRadius(2);
            camera->changeMode(0);
            break;
        default:
            camera->setLookAtPosition(get<0>(teleports[keys[id-15]]));
            camera->setCameraRadius(get<1>(teleports[keys[id-15]]));
            camera->changeMode(0);
            break;
    }
}

/**
 * Creates a menu with options of:
 * - toggling axis
 * - travelling to planet locations
 */
void createMenu(void){
    int submenu1, submenu2, submenu3;
    submenu1 = glutCreateMenu(menu);
    glutAddMenuEntry("NoVbos", 1);
    glutAddMenuEntry("Vbos", 2);
    submenu2 = glutCreateMenu(menu);
    glutAddMenuEntry("GL_FILL", 11);
    glutAddMenuEntry("GL_LINE", 12);
    glutAddMenuEntry("GL_POINT", 13);
    submenu3 = glutCreateMenu(menu);
    int i=0;
    glutAddMenuEntry("Origin", 14);
    for (auto it = teleports.begin(); it != teleports.end(); ++it, i++) {
        glutAddMenuEntry(it->first.c_str(), 15+i);
    }
    glutCreateMenu(menu);
    glutAddSubMenu("Change VBOs", submenu1);
    glutAddSubMenu("Travel To", submenu3);
    glutAddSubMenu("Change polygon mode", submenu2);
    glutAddMenuEntry("Add axes", 10);
    glutAddMenuEntry("Modo DJ", 3);
    glutAddMenuEntry("Show camera info", 9);

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
    camera->updateMouseAngles(button, state, xx, yy);
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
        camera->changeMode(-1);
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
    //XMLDocument* doc;
    tinyxml2::XMLDocument *doc = new  tinyxml2::XMLDocument();
    tinyxml2::XMLError error = doc->LoadFile(filePath);
    tinyxml2::XMLNode* world = doc->FirstChildElement("world");

    float fov = 0, farV = 0, nearV = 0;
    Point* cameraPosition = new Point(), * cameraLookAt = new Point(), * cameraUpVector = new Point();
  
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
                cameraPosition->setX(stof(position->Attribute("x")));
                cameraPosition->setY(stof(position->Attribute("y")));
                cameraPosition->setZ(stof(position->Attribute("z")));
            }

            /* camera lookAt */
            tinyxml2::XMLElement* lookAt = cameraElem->FirstChildElement("lookAt");
            if (lookAt) {
                cameraLookAt->setX(stof(lookAt->Attribute("x")));
                cameraLookAt->setY(stof(lookAt->Attribute("y")));
                cameraLookAt->setZ(stof(lookAt->Attribute("z")));
            }

            /* camera up */
            tinyxml2::XMLElement* up = cameraElem->FirstChildElement("up");
            if (up) {
                cameraUpVector->setX(stof(up->Attribute("x")));
                cameraUpVector->setY(stof(up->Attribute("y")));
                cameraUpVector->setZ(stof(up->Attribute("z")));
            }
            
            /* camera projection */
            tinyxml2::XMLElement* projection = cameraElem->FirstChildElement("projection");
            if (projection) {
                fov = stof(projection->Attribute("fov"));
                nearV = stof(projection->Attribute("near"));
                farV = stof(projection->Attribute("far"));
            }
          
            camera = new Camera(*cameraPosition, *cameraLookAt, *cameraUpVector, fov, nearV, farV);
        }

        group = new Group();
        group->readXML(world->FirstChildElement("group"));

        vector<Transformation*> transf;
        teleports = group->initializeTeleporter(&transf);
        for (auto it = teleports.begin(); it != teleports.end(); ++it) {
            keys.push_back(it->first);
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

            /*for (size_t i = 0; i < 10; i++)
            {
                for (size_t j = 0; j < 10; j++)
                {
                    Point p(i, 0, j);
                    Group* newGroup = Creator::drawCube(*group, p);
                    cubes.emplace_back(*newGroup);
                }
            }*/

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
