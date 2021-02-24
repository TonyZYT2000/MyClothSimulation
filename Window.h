#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "Cube.h"
#include "shader.h"
#include "Camera.h"

////////////////////////////////////////////////////////////////////////////////

class Window
{
public:
	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	// Objects to render
	static Cube* cube;

	// Shader Program 
	static GLuint shaderProgram;

	// Act as Constructors and desctructors 
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	// for the Window
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// update and draw functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// helper to reset the camera
	static void resetCamera();

	// callbacks - for interaction
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_callback(GLFWwindow* window, double currX, double currY);
};

////////////////////////////////////////////////////////////////////////////////

#endif