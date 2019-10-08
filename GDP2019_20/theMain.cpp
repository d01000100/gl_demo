#include "GLCommon.h"
#include "globalStuff.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>		// c libs
#include <stdio.h>		// c libs

#include <iostream>		// C++ IO standard stuff
#include <map>			// Map aka "dictonary" 

#include "cShaderManager.h"

#include <sstream>

// The Physics function
#include "PhysicsStuff.h"
#include "cPhysics.h"

#include "cLowPassFilter.h"

#include "DebugRenderer/cDebugRenderer.h"

#include "Scene.h"
#include "Camera.h"

// Keyboard, error, mouse, etc. are now here
#include "GFLW_callbacks.h"

cShaderManager theShaderManager;
std::string shader_name = "SimpleShader";
Scene* theScene = Scene::getTheScene();

int main(void)
{
	Camera* theCamera = Camera::getTheCamera();
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(640, 480, "SimpleGame", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	cDebugRenderer* pDebugRenderer = new cDebugRenderer();
	pDebugRenderer->initialize();

	cShaderManager::cShader vertexShad;
	vertexShad.fileName = "assets/shaders/vertexShader01.glsl";

	cShaderManager::cShader fragShader;
	fragShader.fileName = "assets/shaders/fragmentShader01.glsl";

	if (!::theShaderManager.createProgramFromFile(::shader_name, vertexShad, fragShader))
	{
		std::cout << "Error: didn't compile the shader" << std::endl;
		std::cout << theShaderManager.getLastError();
		return -1;
	}

	GLuint shaderProgID = ::theShaderManager.getIDFromFriendlyName(::shader_name);

	// Create a VAO Manager...
	cVAOManager* pTheVAOManager = new cVAOManager();
									 
	theScene->loadScene("add file");

	glEnable(GL_DEPTH);			// Write to the depth buffer
	glEnable(GL_DEPTH_TEST);	// Test with buffer when drawing
	
	cPhysics* pPhsyics = new cPhysics();

	cLowPassFilter avgDeltaTimeThingy;

	// Get the initial time
	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		// Get the initial time
		double currentTime = glfwGetTime();

		// Frame time... (how many seconds since last frame)
		double deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		const double SOME_HUGE_TIME = 0.1;	// 100 ms;
		if (deltaTime > SOME_HUGE_TIME)
		{
			deltaTime = SOME_HUGE_TIME;
		}

		avgDeltaTimeThingy.addValue(deltaTime);

		glUseProgram(shaderProgID);

		float ratio;
		int width, height;
		glm::mat4 p, v;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		// Projection matrix
		p = glm::perspective(0.6f,		// FOV
							 ratio,			// Aspect ratio
							 0.1f,			// Near clipping plane
							 1000.0f);		// Far clipping plane

		v = theCamera->lookAt();

		glViewport(0, 0, width, height);

		// Clear both the colour buffer (what we see) and the 
		//  depth (or z) buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		GLint matView_UL = glGetUniformLocation(shaderProgID, "matView");
		GLint matProj_UL = glGetUniformLocation(shaderProgID, "matProj");

		glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));

		theScene->drawScene();

		double averageDeltaTime = avgDeltaTimeThingy.getAverage();
		//pPhsyics->IntegrationStep(::g_vec_pGameObjects, (float)averageDeltaTime);
		
		pDebugRenderer->RenderDebugObjects( v, p, 0.01f );
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}