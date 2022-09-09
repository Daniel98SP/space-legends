#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "Global.h"
#include "client/Client.h"
#include "server/Server.h"
#include "openglframework/Text.h"
#include <irrKlang.h>

using namespace irrklang;
using namespace Global;

//Remove console (only works in Visual Studio)
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

static int previousUpdateTime;
static int previousDrawingTime;

static void keyboardDownCallback(unsigned char key, int x, int y) {	Client::instance().keyPressed(key); }
static void keyboardUpCallback(unsigned char key, int x, int y) { Client::instance().keyReleased(key); }
static void specialDownCallback(int key, int x, int y) { Client::instance().specialKeyPressed(key); }
static void specialUpCallback(int key, int x, int y) { Client::instance().specialKeyReleased(key); }
static void motionCallback(int x, int y) { Client::instance().mouseMove(x, y); }
static void mouseCallback(int button, int state, int x, int y) { Client::instance().mouseClick(state, button, x, y); }
static void drawCallback() {
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = currentTime - previousDrawingTime;
	Client::instance().render(deltaTime);
	glutSwapBuffers();
	previousDrawingTime = currentTime;
}
static void idleCallback() {
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = currentTime - previousUpdateTime;
	
	if (deltaTime > TIME_PER_FRAME)	{
		Server::instance().update(deltaTime);
		Client::instance().update(deltaTime);
		previousUpdateTime = currentTime;
		glutPostRedisplay();
	}
}

static void initShaders() {
	Shader vShader, fShader;
	vShader.initFromFile(VERTEX_SHADER, "shaders/shader.vert");
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/shader.frag");
	shader.init();
	shader.addShader(vShader);
	shader.addShader(fShader);
	shader.link();
	shader.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}


int main(int argc, char **argv) {
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	RESOLUTION = glm::vec2(desktop.right, desktop.bottom);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(RESOLUTION.x, RESOLUTION.y);
	glutCreateWindow(argv[0]);
	glutFullScreen();
	glutDisplayFunc(drawCallback);
	glutIdleFunc(idleCallback);
	glutKeyboardFunc(keyboardDownCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
	glutSpecialFunc(specialDownCallback);
	glutSpecialUpFunc(specialUpCallback);
	glutMouseFunc(mouseCallback);
	glutPassiveMotionFunc(motionCallback);

	glewExperimental = GL_TRUE;
	glewInit();
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initShaders();
	soundEngine = createIrrKlangDevice();
	Text::initClass();
	Server::instance().init();
	Client::instance().init(&Server::instance());
	previousUpdateTime = glutGet(GLUT_ELAPSED_TIME);
	previousDrawingTime = glutGet(GLUT_ELAPSED_TIME);
	glutMainLoop();

	return 0;
}



