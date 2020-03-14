#include "globalStuff.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/string_cast.hpp>

#include <stdlib.h>		// c libs
#include <iostream>		// C++ IO standard stuff
#include "cShaderManager.h"

// The Physics function
#include "cLowPassFilter.h"
#include "DebugRenderer/cDebugRenderer.h"
#include "Scene.h"
#include "Camera.h"
#include "FollowCamera.h"
#include "SceneEditor.h"
#include "JSON_IO.h"
#include "SkyBox.h"

// Keyboard, error, mouse, etc. are now here
#include "GFLW_callbacks.h"
#include "cModelLoader.h"

cShaderManager theShaderManager;
std::string shader_name = "SimpleShader";
std::string scene_filename = "assets/scene1.json";
cVAOManager* theVAOManager = new cVAOManager();
GLFWwindow* ::window = 0;
cBasicTextureManager* ::g_pTextureManager = new cBasicTextureManager();
cDebugRenderer* ::g_pDebugRenderer = new cDebugRenderer();
bool ::isDebug = false, ::isRunning = false, ::withCollisions = true;

int main(void)
{
	Scene* theScene = Scene::getTheScene();
	Camera* theCamera = Camera::getTheCamera();
	SceneEditor *sceneEditor = SceneEditor::getTheEditor();
	SkyBox theSkyBox;
	glm::vec3 cameraOffset(0, 30 ,-50);
	//Gameplay gameplay;

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	::window = glfwCreateWindow(1600, 900, "SimpleGame", NULL, NULL);
	if (!::window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(::window, key_callback);
	glfwMakeContextCurrent(::window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	cDebugRenderer* pDebugRenderer = new cDebugRenderer();
	pDebugRenderer->initialize();
	::g_pDebugRenderer->initialize();

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

	// Load default model for objects (and skybox)
	cModelLoader::LoadPlyModel("assets/models/sphere.ply", cVAOManager::defaultMeshName);
	::theVAOManager->LoadModelIntoVAO(cVAOManager::defaultMeshName, shaderProgID);
	
	if (!readTextures(::scene_filename)) { return -1; }	
	if (!theScene->loadScene(scene_filename)) { return -1; }

	theSkyBox.init(
		"SpaceBox_right1_posX.bmp",
		"SpaceBox_left2_negX.bmp",
		"SpaceBox_top3_posY.bmp",
		"SpaceBox_bottom4_negY.bmp",
		"SpaceBox_front5_posZ.bmp",
		"SpaceBox_back6_negZ.bmp",
		cVAOManager::defaultMeshName);


	sceneEditor->init(theScene);

	glEnable(GL_DEPTH);			// Write to the depth buffer
	glEnable(GL_DEPTH_TEST);	// Test with buffer when drawing

	cLowPassFilter avgDeltaTimeThingy;

	// Get the initial time
	double lastTime = glfwGetTime();
	double flickerTimer = 0;

	//theCamera->setPosition(glm::vec3(0, 100, -150));

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

		int width, height;
		glm::mat4 p, v;

		glfwGetFramebufferSize(window, &width, &height);
		float ratio = width / (float)height;

		// Projection matrix
		p = glm::perspective(0.6f,		// FOV
			ratio,			// Aspect ratio
			0.1f,			// Near clipping plane
			10000.0f);		// Far clipping plane


		glViewport(0, 0, width, height);

		// Clear both the colour buffer (what we see) and the 
		//  depth (or z) buffer. 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		GLint matView_UL = glGetUniformLocation(shaderProgID, "matView");
		GLint matProj_UL = glGetUniformLocation(shaderProgID, "matProj");

		glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));

		double averageDeltaTime = avgDeltaTimeThingy.getAverage();
		theScene->IntegrationStep(averageDeltaTime);

		v = theCamera->lookAt();

		theSkyBox.draw();
		theScene->drawScene();

		sceneEditor->drawDebug();
		if (sceneEditor->getDebugRenderer()) {
			sceneEditor->getDebugRenderer()->RenderDebugObjects(v, p, 0.01f);
		}
		::g_pDebugRenderer->RenderDebugObjects(v, p, averageDeltaTime);
		if (::isDebug) {
			pDebugRenderer->RenderDebugObjects(v, p, averageDeltaTime);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}