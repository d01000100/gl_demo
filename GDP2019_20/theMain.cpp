#include "globalStuff.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/string_cast.hpp>

#include <stdlib.h>		// c libs
#include <stdio.h>		// c libs
#include <iostream>		// C++ IO standard stuff
#include "cShaderManager.h"

// The Physics function
#include "cPhysics.h"
#include "cLowPassFilter.h"
#include "DebugRenderer/cDebugRenderer.h"
#include "Scene.h"
#include "Camera.h"
#include "FollowCamera.h"
#include "SceneEditor.h"
#include "JSON_IO.h"
#include "AABBGrid.h"
#include "DollyCamera.h"
#include "cFBO.h"

// Keyboard, error, mouse, etc. are now here
#include "GFLW_callbacks.h"

// audio things
#include "RenderManager.h"

cShaderManager theShaderManager;
std::string shader_name = "SimpleShader";
std::string scene_filename = "assets/scene1.json";
cVAOManager* theVAOManager = new cVAOManager();
GLFWwindow* ::window = 0;
cBasicTextureManager* ::g_pTextureManager = new cBasicTextureManager();
cDebugRenderer* ::g_pDebugRenderer = new cDebugRenderer();
AABBGrid* pAABBgrid = new AABBGrid();
DollyCamera* dollyCamera = DollyCamera::getTheCamera();
bool ::isDebug = false, ::isRunning = false;
GLuint g_programID = 0;
glm::mat4 viewTransform = glm::mat4(1), projTransform = glm::mat4(1);

int main(void)
{
	Scene* theScene = Scene::getTheScene();
	Camera* theCamera = FollowCamera::getTheCamera();
	SceneEditor *sceneEditor = SceneEditor::getTheEditor();
	glm::vec3 cameraOffset(0, 30 ,-50);

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	int init_width = 1600, init_height = 800;
	::window = glfwCreateWindow(init_width, init_height, "SimpleGame", NULL, NULL);
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

	::g_programID = ::theShaderManager.getIDFromFriendlyName(::shader_name);
	
	if (!readTextures(::scene_filename)) { return -1; }
	if (!theScene->loadScene(scene_filename)) { return -1; }

	//cMesh* cruiseship = theScene->getMeshesMap()["galactica_model"];
	//if (cruiseship) {
	//	pAABBgrid->filterTriangles(cruiseship);
	//}

	//iGameItem* player = theScene->findItem("player");
	//if (player) {
	//	theCamera->init(player, glm::vec3(0, 30, -50));
	//}

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

	cFBO *pTheFBO = new cFBO();
	std::string FBOError;
	if (!pTheFBO->init(init_width, init_height, FBOError))
	{
		printf("FBOError: %s\n", FBOError.c_str());
		return 1;
	}

	while (!glfwWindowShouldClose(window))
	{
		// Doesn't work if all the texture units aren't asigned
		glUniform1i(glGetUniformLocation(g_programID, "skyBox"), 26);	// Texture unit 26
	
		GLint passNumber_UniLoc = glGetUniformLocation(g_programID, "passNumber");
		glUniform1i(passNumber_UniLoc, 0);  //"passNumber"

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

		glUseProgram(g_programID);

		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		// Projection matrix
		::projTransform = glm::perspective(0.6f,		// FOV
			ratio,			// Aspect ratio
			0.1f,			// Near clipping plane
			10000.0f);		// Far clipping plane
		GLint matProj_UL = glGetUniformLocation(g_programID, "matProj");
		glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(::projTransform));

		glViewport(0, 0, width, height);

		//v = theCamera->lookAt();
		GLint matView_UL = glGetUniformLocation(g_programID, "matView");
		//glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));

		double averageDeltaTime = avgDeltaTimeThingy.getAverage();
		//theScene->IntegrationStep(averageDeltaTime);
		//theCamera->reposition();
		sceneEditor->addDebugMarkers();
		
		//theScene->drawScene();
		RenderManager::deferredDraw(
			theCamera->getPosition(),
			theCamera->getTarget(),
			pTheFBO,
			theScene
		);

		// TODO: A Scene has a debug that is called to draw with the Scene.
		if (sceneEditor->getDebugRenderer()) {
			sceneEditor->getDebugRenderer()->RenderDebugObjects(::viewTransform, ::projTransform, 0.01f);
		}
		//::g_pDebugRenderer->RenderDebugObjects(v, p, averageDeltaTime);
		//if (::isDebug) {
		//	pDebugRenderer->RenderDebugObjects(v, p, averageDeltaTime);
		//}
		

		// ===========================
		// ====== Second pass ========
		// ===========================

		// 1. Disable de FBO
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. Clear the ACTUAL frame buffer
		// Clear both the colour buffer (what we see) and the 
		//  depth (or z) buffer. 
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 3. Use the FBO colour texture as the texture on that quad
		glUniform1i(passNumber_UniLoc, 1);  //"passNumber"

		// Tie the texture to the texture unitd
		glActiveTexture(GL_TEXTURE0 + 40);				// Texture Unit 40!!
		glBindTexture(GL_TEXTURE_2D, pTheFBO->colourTexture_0_ID);	// Texture now assoc with texture unit 40
		GLint textSamp00_UL = glGetUniformLocation(g_programID, "secondPassColourTexture");
		glUniform1i(textSamp00_UL, 40);	// Texture unit 40

		// 4. Move the camera
		//v = glm::lookAt(glm::vec3(0,0,-100), glm::vec3(0,0,0), glm::vec3(0,1,0));
		//glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		//glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));

		// 5. Draw a single object
		cGameObject* sceneObj = new cGameObject();
		sceneObj->isVisible = true;
		sceneObj->isLit = false;
		sceneObj->meshName = "quad_model";
		sceneObj->setPos(glm::vec3(0));
		sceneObj->scale = 100.0f;
		Scene* pFinalScene = new Scene();
		pFinalScene->addItem(sceneObj);
		pFinalScene->pSkyBox = nullptr;

		RenderManager::deferredDraw(
			glm::vec3(0, 0, -100),
			glm::vec3(0, 0, 0),
			nullptr,
			pFinalScene
		);

		// 6. Get the screen size and send it to the shader
		// Get the "screen" framebuffer size 
		glfwGetFramebufferSize(window, &width, &height);
		GLint screenWidth_UnitLoc = glGetUniformLocation(g_programID, "screenWidth");
		GLint screenHeight_UnitLoc = glGetUniformLocation(g_programID, "screenHeight");
		glUniform1f(screenWidth_UnitLoc, width);
		glUniform1f(screenHeight_UnitLoc, height);

		// ============= End of second pass ================

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}