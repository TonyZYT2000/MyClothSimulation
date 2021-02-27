////////////////////////////////////////
// Window.cpp
////////////////////////////////////////

#include "Window.h"

////////////////////////////////////////////////////////////////////////////////
// Pause indicator
bool Window::pause = true;
// Wire mode, 0 face 1 wire
bool Window::wireMode = false;
// Culling mode, 0 off 1 on
bool Window::cullingMode = false;

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "CSE 169 Project 4";

// Objects to render
Land* Window::land;
Cloth* Window::cloth;

// Wind
glm::vec3 Window::wind = glm::vec3(0);

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;


////////////////////////////////////////////////////////////////////////////////

// Constructors and desctructors 
bool Window::initializeProgram() {

	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	land = new Land(100, glm::vec3(0, -10, 0));
	cloth = new Cloth(60, 60, glm::vec3(0, 0, 0), land);
	cloth->blow(wind);

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete land;
	delete cloth;

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}

////////////////////////////////////////////////////////////////////////////////

// for the Window
GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// set up the camera
	Cam = new Camera();
	Cam->SetAspect(float(width) / float(height));

	// initialize the interaction variables
	LeftDown = RightDown = false;
	MouseX = MouseY = 0;

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	Cam->SetAspect(float(width) / float(height));
}

////////////////////////////////////////////////////////////////////////////////

// update and draw functions
void Window::idleCallback()
{
	// Perform any updates as necessary. 
	Cam->Update();

	if (!pause) {
            cloth->update();
            cloth->update();
            cloth->update();
            cloth->update();
            cloth->update();
            cloth->update();
            cloth->update();
            cloth->update();
            cloth->update();
            cloth->update();
	}
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render the object.
	land->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
	cloth->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

////////////////////////////////////////////////////////////////////////////////

// helper to reset the camera
void Window::resetCamera() 
{
	Cam->Reset();
	Cam->SetAspect(float(Window::width) / float(Window::height));
}

////////////////////////////////////////////////////////////////////////////////

// callbacks - for Interaction 
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press.
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key) 
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		case GLFW_KEY_R:
			resetCamera();
			break;

		// pause playback
		case GLFW_KEY_SPACE:
			pause = !pause;
			break;

		case GLFW_KEY_BACKSPACE:
			cloth->toggleFree();
			break;

		// toggle show face or show wire frame
		case GLFW_KEY_P:
			wireMode = !wireMode;
			if (wireMode) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			break;

		// toggle culling
		case GLFW_KEY_C:
			cullingMode = !cullingMode;
			if (cullingMode) {
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}
			else {
				glDisable(GL_CULL_FACE);
			}
			break;

		// wind control
		case GLFW_KEY_UP:
			wind -= glm::vec3(0, 0, 0.5);
			cloth->blow(wind);
			std::cerr << "Wind Velocity: " << "(" <<
				wind.x << ", " <<
				wind.y << ", " <<
				wind.z << ")" << std::endl;
			break;

		case GLFW_KEY_DOWN:
			wind += glm::vec3(0, 0, 0.5);
			cloth->blow(wind);
			std::cerr << "Wind Velocity: " << "(" <<
				wind.x << ", " <<
				wind.y << ", " <<
				wind.z << ")" << std::endl;
			break;

		case GLFW_KEY_LEFT:
			wind -= glm::vec3(0.5, 0, 0);
			cloth->blow(wind);
			std::cerr << "Wind Velocity: " << "(" <<
				wind.x << ", " <<
				wind.y << ", " <<
				wind.z << ")" << std::endl;
			break;

		case GLFW_KEY_RIGHT:
			wind += glm::vec3(0.5, 0, 0);
			cloth->blow(wind);
			std::cerr << "Wind Velocity: " << "(" <<
				wind.x << ", " <<
				wind.y << ", " <<
				wind.z << ")" << std::endl;
			break;

		case GLFW_KEY_RIGHT_SHIFT:
			wind += glm::vec3(0, 0.5, 0);
			cloth->blow(wind);
			std::cerr << "Wind Velocity: " << "(" <<
				wind.x << ", " <<
				wind.y << ", " <<
				wind.z << ")" << std::endl;
			break;

		case GLFW_KEY_RIGHT_CONTROL:
			wind -= glm::vec3(0, 0.5, 0);
			cloth->blow(wind);
			std::cerr << "Wind Velocity: " << "(" <<
				wind.x << ", " <<
				wind.y << ", " <<
				wind.z << ")" << std::endl;
			break;

		case GLFW_KEY_W:
			cloth->translate(glm::vec3(0, 0, -0.05));
			break;

		case GLFW_KEY_S:
			cloth->translate(glm::vec3(0, 0, 0.05));
			break;

		case GLFW_KEY_A:
			cloth->translate(glm::vec3(-0.05, 0, 0));
			break;

		case GLFW_KEY_D:
			cloth->translate(glm::vec3(0.05, 0, 0));
			break;

		case GLFW_KEY_LEFT_SHIFT:
			cloth->translate(glm::vec3(0, 0.05, 0));
			break;

		case GLFW_KEY_LEFT_CONTROL:
			cloth->translate(glm::vec3(0, -0.05, 0));
			break;

		case GLFW_KEY_Q:
			cloth->rotate(glm::radians(1.0f));
			break;

		case GLFW_KEY_E:
			cloth->rotate(glm::radians(-1.0f));
			break;

		default:
			break;
		}
	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		LeftDown = (action == GLFW_PRESS);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		RightDown = (action == GLFW_PRESS);
	}
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {

	int maxDelta = 100;
	int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

	MouseX = (int)currX;
	MouseY = (int)currY;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if (LeftDown) {
		const float rate = 1.0f;
		Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
		Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
	}
	if (RightDown) {
		const float rate = 0.005f;
		float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
		Cam->SetDistance(dist);
	}
}

////////////////////////////////////////////////////////////////////////////////
