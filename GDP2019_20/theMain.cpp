#include "GLCommon.h"
#include "globalStuff.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/string_cast.hpp>

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
#include "quaternions_utils.h"
#include "DollyCamera.h"

// Keyboard, error, mouse, etc. are now here
#include "GFLW_callbacks.h"
#include "cSimpleAssimpSkinnedMeshLoader_OneMesh.h"

cShaderManager theShaderManager;
std::string shader_name = "SimpleShader";
GLuint shaderProgID = 0;
std::string scene_filename = "assets/scene1.json";
cVAOManager* theVAOManager = new cVAOManager();
GLFWwindow* ::window = 0;
cBasicTextureManager* ::g_pTextureManager = new cBasicTextureManager();
cDebugRenderer* ::g_pDebugRenderer = new cDebugRenderer();
AABBGrid* pAABBgrid = new AABBGrid();
DollyCamera* dollyCamera = DollyCamera::getTheCamera();
bool ::isDebug = false, ::isRunning = false;

int main(void)
{	Scene* theScene = Scene::getTheScene();
	Camera* theCamera = FollowCamera::getTheCamera();
	SceneEditor *sceneEditor = SceneEditor::getTheEditor();
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

	::shaderProgID = ::theShaderManager.getIDFromFriendlyName(::shader_name);
	
	if (!readTextures(::scene_filename)) { return -1; }
	/*
	 * Skin Mesh test
	 */
	//cGameObject* rpgPlayer = new cGameObject();
	//rpgPlayer->skinnedMesh = new cSimpleAssimpSkinnedMesh();
	//auto pSM = rpgPlayer->skinnedMesh;
	//if (!pSM->LoadMeshFromFile("RPG Player bind pose", "assets/fbx/models/rpg_player.fbx"))
	//{
	//	std::cout << "Skinned mesh model not found!!\n";
	//	return 1;
	//};
	//if (!pSM->LoadMeshAnimation("kickL1", "assets/fbx/animation/kick-L1.fbx"))
	//{
	//	std::cout << "Animation not Found!!\n";
	//	return 1;
	//}
	//sModelDrawInfo* pDI = pSM->CreateModelDrawInfoObjectFromCurrentModel();
	//::theVAOManager->LoadModelDrawInfoIntoVAO(*pDI, shaderProgID);
	//rpgPlayer->meshName = "RPG Player bind pose";
	//rpgPlayer->friendlyName = "rpgPlayer";
	//rpgPlayer->pitch(-90);
	
	if (!theScene->loadScene(scene_filename)) { return -1; }

	theSkyBox.init(
		"SpaceBox_right1_posX.bmp",
		"SpaceBox_left2_negX.bmp",
		"SpaceBox_top3_posY.bmp",
		"SpaceBox_bottom4_negY.bmp",
		"SpaceBox_front5_posZ.bmp",
		"SpaceBox_back6_negZ.bmp",
		"sphere_model");

	//theScene->addItem(rpgPlayer);


	sceneEditor->init(theScene);

	glEnable(GL_DEPTH);			// Write to the depth buffer
	glEnable(GL_DEPTH_TEST);	// Test with buffer when drawing
	
	cPhysics* pPhysics = new cPhysics();
	pPhysics->debugRenderer = pDebugRenderer;

	cLowPassFilter avgDeltaTimeThingy;

	// Get the initial time
	double lastTime = glfwGetTime();
	double flickerTimer = 0;

	theCamera->setPosition(glm::vec3(0, 100, -150));

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


		glViewport(0, 0, width, height);

		// Clear both the colour buffer (what we see) and the 
		//  depth (or z) buffer. 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		GLint matView_UL = glGetUniformLocation(shaderProgID, "matView");
		GLint matProj_UL = glGetUniformLocation(shaderProgID, "matProj");

		glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));

		double averageDeltaTime = avgDeltaTimeThingy.getAverage();
		//theScene->IntegrationStep(averageDeltaTime);
		//theCamera->reposition();

		aGameItem* player = theScene->findItem("player");
		if (player) {
			//BroadCollision::collisionsReact(
			//	BroadCollision::detectCollisions(pAABBgrid, (cGameObject*)player),
			//	(cGameObject*)player);
			//pAABBgrid->Draw(player->getPos());
		}
		//pAABBgrid->Draw();

		v = theCamera->lookAt();

		if (::isRunning)
		{
			theCamera->setTarget(theScene->findItem("player")->getPos());
			v = dollyCamera->lookAt();
		}

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