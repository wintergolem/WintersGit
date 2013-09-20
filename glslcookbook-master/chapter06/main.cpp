#include "cookbookogl.h"
#include <GL/glfw.h>

#include "glutils.h"
#include "scenebezcurve.h"
#include "scenepointsprite.h"
#include "scenequadtess.h"
#include "sceneshadewire.h"
#include "scenesilhouette.h"
#include "scenetessteapot.h"
#include "scenetessteapotdepth.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

Scene *scene;

string parseCLArgs(int argc, char ** argv);
void printHelpInfo(const char *);

void initializeGL() {
    glClearColor(0.5f,0.5f,0.5f,1.0f);
    scene->initScene();
}

void mainLoop() {
	while( glfwGetWindowParam(GLFW_OPENED) && !glfwGetKey(GLFW_KEY_ESC) ) {
		GLUtils::checkForOpenGLError(__FILE__,__LINE__);
		scene->update(glfwGetTime());
		scene->render();
		glfwSwapBuffers();
	}
}

void resizeGL(int w, int h ) {
    scene->resize(w,h);
}

int main(int argc, char *argv[])
{
	string recipe = parseCLArgs(argc, argv);

	// Initialize GLFW
	if( !glfwInit() ) exit( EXIT_FAILURE );

	// Select OpenGL 3.2 with a forward compatible core profile.
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR, 4 );
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MINOR, 3 );
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 8);

	// Open the window
	if( !glfwOpenWindow( WIN_WIDTH, WIN_HEIGHT, 8,8,8,8,24,0, GLFW_WINDOW ) ) {
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	string title = "Chapter 6 -- " + recipe;
	glfwSetWindowTitle(title.c_str());

	// Load the OpenGL functions.
	if( ogl_LoadFunctions() == ogl_LOAD_FAILED ) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	GLUtils::dumpGLInfo();

	// Initialization
	initializeGL();
	resizeGL(WIN_WIDTH,WIN_HEIGHT);

	// Enter the main loop
	mainLoop();

	// Close window and terminate GLFW
	glfwTerminate();
	// Exit program
	exit( EXIT_SUCCESS );
}

string parseCLArgs(int argc, char ** argv) {

	if( argc < 2 ) {
		printHelpInfo(argv[0]);
		exit(EXIT_FAILURE);
	}

	string recipe = argv[1];

	if( recipe == "bez-curve" ) {
		scene = new SceneBezCurve();
	} else if( recipe == "point-sprite") {
		scene = new ScenePointSprite();
	} else if( recipe == "quad-tess") {
		scene = new SceneQuadTess();
	} else if( recipe == "shade-wire" ) {
		scene = new SceneShadeWire();
	} else if( recipe == "silhouette") {
		scene = new SceneSilhouette();
	} else if( recipe == "tess-teapot" ) {
		scene = new SceneTessTeapot();
	} else if( recipe == "tess-teapot-depth" ) {
		scene = new SceneTessTeapotDepth();
	} else {
		printf("Unknown recipe: %s\n", recipe.c_str());
		printHelpInfo(argv[0]);
		exit(EXIT_FAILURE);
	}

	return recipe;
}

void printHelpInfo(const char * exeFile) {
	printf("Usage: %s recipe-name\n\n", exeFile);
	printf("Recipe names: \n");
	printf("  bez-curve         : description...\n");
	printf("  point-sprite      : description...\n");
	printf("  quad-tess         : description...\n");
	printf("  shade-wire        : description...\n");
	printf("  silhouette        : description...\n");
	printf("  tess-teapot       : description...\n");
	printf("  tess-teapot-depth : description...\n");
}

