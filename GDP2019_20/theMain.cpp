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

#include "cModelLoader.h"			
#include "cVAOManager.h"		// NEW
#include "cGameObject.h"

#include "cShaderManager.h"

#include <sstream>

#include <limits.h>
#include <float.h>

// The Physics function
#include "PhysicsStuff.h"
#include "cPhysics.h"

#include "cLowPassFilter.h"

#include "DebugRenderer/cDebugRenderer.h"

// Used to visualize the attenuation of the lights...
#include "LightManager/cLightHelper.h"

// Keyboard, error, mouse, etc. are now here
#include "GFLW_callbacks.h"
#include "Scene.h"

glm::vec3 cameraEye = glm::vec3(0.0, 80.0, -80.0);
glm::vec3 cameraTarget = glm::vec3(0.0f, 10.0, 0.0f);
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

cShaderManager theShaderManager;
std::string shader_name = "SimpleShader";
Scene* theScene = Scene::getTheScene();

glm::vec3 sexyLightPosition = glm::vec3(-25.0f,30.0f,0.0f);
float sexyLightConstAtten = 0.0000001f;			// not really used (can turn off and on the light)
float sexyLightLinearAtten = 0.03f;  
float sexyLightQuadraticAtten = 0.0000001f;

int main(void)
{
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

	
	cModelLoader* pTheModelLoader = new cModelLoader();	// Heap

	// Create a VAO Manager...
	cVAOManager* pTheVAOManager = new cVAOManager();
									 
	theScene->loadScene("add file");

	glEnable(GL_DEPTH);			// Write to the depth buffer
	glEnable(GL_DEPTH_TEST);	// Test with buffer when drawing
	
	cPhysics* pPhsyics = new cPhysics();

	cLowPassFilter avgDeltaTimeThingy;

	cLightHelper* pLightHelper = new cLightHelper();

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

		// View matrix
		v = glm::mat4(1.0f);

		v = glm::lookAt(cameraEye,
						cameraTarget,
						upVector);

		glViewport(0, 0, width, height);

		// Clear both the colour buffer (what we see) and the 
		//  depth (or z) buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


		// Set the lighting values for the shader. There is only 1 light right now.
		// uniform vec4 theLights[0].position
		// uniform vec4 theLights[0].diffuse
		// uniform vec4 theLights[0].specular
		// uniform vec4 theLights[0].atten
		// uniform vec4 theLights[0].direction
		// uniform vec4 theLights[0].param1
		// uniform vec4 theLights[0].param2		
		//GLint L_0_position = glGetUniformLocation( shaderProgID, "theLights[0].position");
		//GLint L_0_diffuse = glGetUniformLocation( shaderProgID, "theLights[0].diffuse");
		//GLint L_0_specular = glGetUniformLocation( shaderProgID, "theLights[0].specular");
		//GLint L_0_atten = glGetUniformLocation( shaderProgID, "theLights[0].atten");
		//GLint L_0_direction = glGetUniformLocation( shaderProgID, "theLights[0].direction");
		//GLint L_0_param1 = glGetUniformLocation( shaderProgID, "theLights[0].param1");
		//GLint L_0_param2 = glGetUniformLocation( shaderProgID, "theLights[0].param2");

		//glUniform4f(L_0_position, 
		//			sexyLightPosition.x,
		//			sexyLightPosition.y,
		//			sexyLightPosition.z,
		//			1.0f);
		//glUniform4f(L_0_diffuse, 1.0f, 1.0f, 1.0f, 1.0f );	// White
		//glUniform4f(L_0_specular, 1.0f, 1.0f, 1.0f, 1.0f );	// White
		//glUniform4f(L_0_atten, 0.0f,  // constant attenuation
		//			           sexyLightLinearAtten,  // Linear 
		//			           sexyLightQuadraticAtten,	// Quadratic 
		//			           1000000.0f );	// Distance cut off

		//// Point light:
		//glUniform4f(L_0_param1, 0.0f /*POINT light*/, 0.0f, 0.0f, 1.0f );
		//
		//glUniform4f(L_0_param2, 1.0f /*Light is on*/, 0.0f, 0.0f, 1.0f );

		// Also set the position of my "eye" (the camera)
		//uniform vec4 eyeLocation;
		GLint eyeLocation_UL = glGetUniformLocation( shaderProgID, "eyeLocation");

		glUniform4f( eyeLocation_UL, 
					 cameraEye.x, cameraEye.y, cameraEye.z, 1.0f );


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