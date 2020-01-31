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
#include "cPhysics.h"
#include "cLowPassFilter.h"
#include "DebugRenderer/cDebugRenderer.h"
#include "Scene.h"
#include "Camera.h"
#include "FollowCamera.h"
#include "SceneEditor.h"
#include "JSON_IO.h"
#include "SkyBox.h"
#include "AABBGrid.h"
#include "DollyCamera.h"
#include "PhysicsConfigs.h"

// Keyboard, error, mouse, etc. are now here
#include "GFLW_callbacks.h"

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
	
	//glm::vec3 vector(1, 2, 2);-
	//glm::quat orientation(glm::vec3(0,1.6,0));


	//glm::vec3 v1(0, -1, 0), v2(-1, -1, -1);
	//glm::quat rotation = RotationBetweenVectors(v1, v2);
	//printf("Setting direction of test at: %s. quat: %s\nOriginally looking at %s\nNow looking at %s\n\n",
	//	glm::to_string(v2).c_str(),
	//	glm::to_string(rotation).c_str(),
	//	glm::to_string(v1).c_str(),
	//	glm::to_string(rotation * v1).c_str());

	//float min = 5.0f, max = 15.0f;
	//for (float val = min; val <= max; val += 0.1f) {
	//	float step = glm::smoothstep(max, min, val);
	//	printf("min %f, max %f, value %f, result %f\n",
	//		min, max, val, step);
	//}

	::g_initPhysics();
	nPhysics::sBallDef ballDef;
	ballDef.Position = glm::vec3(0,10,0);
	nPhysics::iBallComponent *ball = ::g_PhysicsFactory->CreateBall(ballDef);

	nPhysics::sPlaneDef planeDef;
	planeDef.Normal = glm::vec3(0, 1, 0);
	planeDef.Point = glm::vec3(5);
	nPhysics::iPhysicsComponent* plane = ::g_PhysicsFactory->CreatePlane(planeDef);

	::g_PhysicsWorld->AddComponent(ball);
	::g_PhysicsWorld->AddComponent(plane);

	float deltaTime = 0.1f;

	for (int i = 0;i < 40;i++)
	{
		//sphereBody.ApplyForce(glm::vec3(0,0,1));
		::g_PhysicsWorld->Update(deltaTime);
		std::cout << "Sphere 1: ";
		std::cout << ball->ToString() << std::endl;
		//std::cout << "Sphere 2: ";
		//phys::Util::PrintRigidBody(sphere2Body);
		printf("\n");
	}
	
	::g_PhysicsWorld->RemoveComponent(ball);
	::g_PhysicsWorld->RemoveComponent(plane);

	::g_destroyPhysics();

	return 0;

	Scene* theScene = Scene::getTheScene();
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


	GLuint shaderProgID = ::theShaderManager.getIDFromFriendlyName(::shader_name);
	
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

	theSkyBox.init(
		"SpaceBox_right1_posX.bmp",
		"SpaceBox_left2_negX.bmp",
		"SpaceBox_top3_posY.bmp",
		"SpaceBox_bottom4_negY.bmp",
		"SpaceBox_front5_posZ.bmp",
		"SpaceBox_back6_negZ.bmp",
		"sphere_model");


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