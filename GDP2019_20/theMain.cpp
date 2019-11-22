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
#include "util.h"

// The Physics function
#include "PhysicsStuff.h"
#include "cPhysics.h"
#include "cLowPassFilter.h"
#include "DebugRenderer/cDebugRenderer.h"
#include "Scene.h"
#include "Camera.h"
#include "FollowCamera.h"
#include "SceneEditor.h"
#include "JSON_IO.h"
#include "cLight.h"
#include "SkyBox.h"
#include "AABBGrid.h"
#include "colors.h"
#include "BroadCollision.h"

// Keyboard, error, mouse, etc. are now here
#include "GFLW_callbacks.h"

// audio things
#include "audio_item.h"
#include "audio_listener.h"

cShaderManager theShaderManager;
std::string shader_name = "SimpleShader";
std::string scene_filename = "assets/scene1.json";
cVAOManager* theVAOManager = new cVAOManager();
GLFWwindow* ::window = 0;
cBasicTextureManager* ::g_pTextureManager = new cBasicTextureManager();
cDebugRenderer* ::g_pDebugRenderer = new cDebugRenderer();
AABBGrid* pAABBgrid = new AABBGrid();
bool ::isDebug = false;

// audio globals
FMOD::System *::fmod_system = 0;

bool init_fmod() {
	//Create system
	error_check(FMOD::System_Create(&::fmod_system));
	//Init system
	error_check(::fmod_system->init(32, FMOD_INIT_NORMAL, 0));

	error_check(::fmod_system->set3DSettings(1.0f, 1.0f, 1.0f));

	return true;
}

int main(void)
{
	//AABBHash(glm::vec3(-4580.1546, -0.1579, 126.12));
	//std::vector<int> v1, v2;
	//v1.push_back(1); v1.push_back(4); v1.push_back(5);
	//v2.push_back(4); v2.push_back(5);
	//stdVecConc(&v1, &v2);
	//for (int i = 0; i < v1.size(); i++)
	//	printf("%d, ", v1[i]);
	//sNiceTriangle* tri = new sNiceTriangle();
	//tri->a = glm::vec3(0);
	//tri->b = glm::vec3(0,0,500);
	//tri->c = glm::vec3(500);
	//std::vector<unsigned long long> hashes = getTriangleHashes(tri);
	//return 0;

	Scene* theScene = Scene::getTheScene();
	FollowCamera* theCamera = FollowCamera::getPhysicsCamera();
	SceneEditor *sceneEditor = SceneEditor::getTheEditor();
	init_fmod();
	SkyBox theSkyBox;
	glm::vec3 cameraOffset(0, 30 ,-50);

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	::window = glfwCreateWindow(1600, 800, "SimpleGame", NULL, NULL);
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
	
	if (!readTextures(::scene_filename)) { return -1; }
	if (!theScene->loadScene(scene_filename)) { return -1; }
	cMesh* cruiseship = theScene->getMeshesMap()["galactica_model"];
	if (cruiseship) {
		pAABBgrid->filterTriangles(cruiseship);
	}

	iGameItem* player = theScene->findItem("player");
	if (player) {
		theCamera->init(player, glm::vec3(0, 30, -50));
	}

	theSkyBox.init(
		"SpaceBox_right1_posX.bmp",
		"SpaceBox_left2_negX.bmp",
		"SpaceBox_top3_posY.bmp",
		"SpaceBox_bottom4_negY.bmp",
		"SpaceBox_front5_posZ.bmp",
		"SpaceBox_back6_negZ.bmp",
		"sphere_model");

	//sceneEditor->init(theScene);

	glEnable(GL_DEPTH);			// Write to the depth buffer
	glEnable(GL_DEPTH_TEST);	// Test with buffer when drawing
	
	cPhysics* pPhysics = new cPhysics();
	pPhysics->debugRenderer = pDebugRenderer;

	cLowPassFilter avgDeltaTimeThingy;

	// Get the initial time
	double lastTime = glfwGetTime();
	double flickerTimer = 0;

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
							 10000.0f);		// Far clipping plane

		v = theCamera->lookAt();

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
		theCamera->reposition();

		iGameItem* player = theScene->findItem("player");
		if (player) {
			BroadCollision::collisionsReact(
				BroadCollision::detectCollisions(pAABBgrid, (cGameObject*)player),
				(cGameObject*)player);
			//pAABBgrid->Draw(player->getPos());
		}
		//pAABBgrid->Draw();

		theSkyBox.draw();

		theScene->drawScene();
		sceneEditor->drawDebug();
		if (sceneEditor->getDebugRenderer()) {
			sceneEditor->getDebugRenderer()->RenderDebugObjects( v, p, 0.01f );
		}
		if (::isDebug) {
			::g_pDebugRenderer->RenderDebugObjects(v, p, averageDeltaTime);
			pDebugRenderer->RenderDebugObjects(v, p, averageDeltaTime);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();

		error_check(::fmod_system->update());

	}
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}