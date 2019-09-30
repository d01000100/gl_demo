#include "GLCommon.h"
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>

//#include "linmath.h"
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

#include "DebugRenderer/cDebugRenderer.h"

// Used to visualize the attenuation of the lights...
#include "LightManager/cLightHelper.h"


void DrawObject(glm::mat4 m,
				cGameObject* pCurrentObject,
				GLint shaderProgID,
				cVAOManager* pVAOManager);


glm::vec3 cameraEye = glm::vec3(0.0, 80.0, -80.0);
glm::vec3 cameraTarget = glm::vec3(0.0f, 10.0, 0.0f);
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 sexyLightPosition = glm::vec3(-25.0f,30.0f,0.0f);
float sexyLightConstAtten = 0.0000001f;			// not really used (can turn off and on the light)
float sexyLightLinearAtten = 0.03f;  
float sexyLightQuadraticAtten = 0.0000001f;

float sexyLightSpotInnerAngle = 5.0f;
float sexyLightSpotOuterAngle = 7.5f;
// This is a "normalized" direction
// (i.e. the length is 1.0f)
glm::vec3 sexyLightSpotDirection = glm::vec3(0.0f, -1.0f, 0.0f);

bool bLightDebugSheresOn = true;


// Load up my "scene"  (now global)
std::vector<cGameObject*> g_vec_pGameObjects;
std::map<std::string /*FriendlyName*/, cGameObject*> g_map_GameObjectsByFriendlyName;


// returns NULL (0) if we didn't find it.
cGameObject* pFindObjectByFriendlyName( std::string name );
cGameObject* pFindObjectByFriendlyNameMap( std::string name );

//bool g_BallCollided = false;

bool isShiftKeyDownByAlone(int mods)
{
	if (mods == GLFW_MOD_SHIFT)			
	{
		// Shift key is down all by itself
		return true;
	}
	//// Ignore other keys and see if the shift key is down
	//if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
	//{

	//}
	return false;
}

bool isCtrlKeyDownByAlone(int mods)
{
	if (mods == GLFW_MOD_CONTROL)			
	{
		return true;
	}
	return false;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	const float CAMERASPEED = 2.0f;

	if ( !isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods) )
	{

		// Move the camera (A & D for left and right, along the x axis)
		if (key == GLFW_KEY_A)
		{
			cameraEye.x -= CAMERASPEED;		// Move the camera -0.01f units
		}
		if (key == GLFW_KEY_D)
		{
			cameraEye.x += CAMERASPEED;		// Move the camera +0.01f units
		}

		// Move the camera (Q & E for up and down, along the y axis)
		if (key == GLFW_KEY_Q)
		{
			cameraEye.y -= CAMERASPEED;		// Move the camera -0.01f units
		}
		if (key == GLFW_KEY_E)
		{
			cameraEye.y += CAMERASPEED;		// Move the camera +0.01f units
		}

		// Move the camera (W & S for towards and away, along the z axis)
		if (key == GLFW_KEY_W)
		{
			cameraEye.z -= CAMERASPEED;		// Move the camera -0.01f units
		}
		if (key == GLFW_KEY_S)
		{
			cameraEye.z += CAMERASPEED;		// Move the camera +0.01f units
		}

		if ( key == GLFW_KEY_B )
		{ 
			// Shoot a bullet from the pirate ship
			// Find the pirate ship...
			// returns NULL (0) if we didn't find it.
//			cGameObject* pShip = pFindObjectByFriendlyName("PirateShip");
			cGameObject* pShip = pFindObjectByFriendlyNameMap("PirateShip");
			// Maybe check to see if it returned something... 

			// Find the sphere#2
//			cGameObject* pBall = pFindObjectByFriendlyName("Sphere#2");
			cGameObject* pBall = pFindObjectByFriendlyNameMap("Sphere#2");

			// Set the location velocity for sphere#2
			pBall->positionXYZ = pShip->positionXYZ;
			pBall->inverseMass = 1.0f;		// So it's updated
			// 20.0 units "to the right"
			// 30.0 units "up"
			pBall->velocity = glm::vec3( 15.0f, 20.0f, 0.0f );
			pBall->accel = glm::vec3(0.0f,0.0f,0.0f);
			pBall->diffuseColour = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		}//if ( key == GLFW_KEY_B )

	}

	if (isShiftKeyDownByAlone(mods))
	{
		// move the light
		if (key == GLFW_KEY_A)
		{
			sexyLightPosition.x -= CAMERASPEED;		// Move the camera -0.01f units
		}
		if (key == GLFW_KEY_D)
		{
			sexyLightPosition.x += CAMERASPEED;		// Move the camera +0.01f units
		}

		// Move the camera (Q & E for up and down, along the y axis)
		if (key == GLFW_KEY_Q)
		{
			sexyLightPosition.y -= CAMERASPEED;		// Move the camera -0.01f units
		}
		if (key == GLFW_KEY_E)
		{
			sexyLightPosition.y += CAMERASPEED;		// Move the camera +0.01f units
		}

		// Move the camera (W & S for towards and away, along the z axis)
		if (key == GLFW_KEY_W)
		{
			sexyLightPosition.z -= CAMERASPEED;		// Move the camera -0.01f units
		}
		if (key == GLFW_KEY_S)
		{
			sexyLightPosition.z += CAMERASPEED;		// Move the camera +0.01f units
		}

		if (key == GLFW_KEY_1)
		{
			sexyLightConstAtten *= 0.99f;			// 99% of what it was
		}
		if (key == GLFW_KEY_2)
		{
			sexyLightConstAtten *= 1.01f;			// 1% more of what it was
		}		
		if (key == GLFW_KEY_3)
		{
			sexyLightLinearAtten *= 0.99f;			// 99% of what it was
		}
		if (key == GLFW_KEY_4)
		{
			sexyLightLinearAtten *= 1.01f;			// 1% more of what it was
		}
		if (key == GLFW_KEY_5)
		{
			sexyLightQuadraticAtten *= 0.99f;			// 99% of what it was
		}
		if (key == GLFW_KEY_6)
		{
			sexyLightQuadraticAtten *= 1.01f;			// 1% more of what it was
		}
		if (key == GLFW_KEY_V)
		{
			sexyLightSpotInnerAngle -= 0.1f;
		}
		if (key == GLFW_KEY_B)
		{
			sexyLightSpotInnerAngle += 0.1f;
		}
		if (key == GLFW_KEY_N)
		{
			sexyLightSpotOuterAngle -= 0.1f;
		}
		if (key == GLFW_KEY_M)
		{
			sexyLightSpotOuterAngle += 0.1f;
		}


		if (key == GLFW_KEY_9)
		{
			bLightDebugSheresOn = false;			
		}
		if (key == GLFW_KEY_0)
		{
			bLightDebugSheresOn = true; 
		}

	}//if (isShiftKeyDownByAlone(mods))

	if (isCtrlKeyDownByAlone(mods))
	{
		cGameObject* pShip = pFindObjectByFriendlyName("PirateShip");
		// Turn the ship around
		if (key == GLFW_KEY_A)
		{	// Left
			pShip->HACK_AngleAroundYAxis -= 0.01f;
			pShip->rotationXYZ.y = pShip->HACK_AngleAroundYAxis;
		}
		if (key == GLFW_KEY_D)
		{	// Right
			pShip->HACK_AngleAroundYAxis += 0.01f;
			pShip->rotationXYZ.y = pShip->HACK_AngleAroundYAxis;
		}
		if (key == GLFW_KEY_W)
		{	// Faster
			pShip->HACK_speed += 0.1f;
		}
		if (key == GLFW_KEY_S)
		{	// Slower
			pShip->HACK_speed -= 0.1f;
		}
	}




	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}




int main(void)
{
	//int myArray[15];		// Integers stores integers

	//std::vector<int> myVector;
	//myVector.push_back( 25 );		// 0
	//myVector.push_back(  8 );		// 1
	//myVector.push_back( 17 );		// 2
	//myVector.push_back(  1 );
	//myVector.push_back( 11 );		// Ball
	//myVector.push_back(  6 );
	//myVector.push_back( 15 );
	//myVector.push_back( 25 );		// Pirate
	//myVector.push_back( 22 );
	//myVector.push_back( 27 );
	//myVector.push_back( 13 );
	//std::cout << myVector[3];

	//std::map<int, int> myMap;
	//myMap[0] = 25;		// 0
	//myMap[1] =  8;		// 1
	//myMap[2] = 17;		// 2
	//myMap[3] =  1;
	//myMap[4] = 11;		
	//myMap[5] =  6;
	//myMap[6] = 15;
	//myMap[7] = 25;	
	//myMap[8] = 22;
	//myMap[9] = 27;
	//myMap[10] = 13;		// I WANT THIS ONE
	//myMap.find( 13 );

	//std::map<std::string, cGameObject> myMap;
	//myMap["PirateShip"]


	// A map for favorite foods
	std::map< std::string /*index*/, std::string > mapNameToFood;

	// 11 -> 2.3
	// 111 -> 4.7
	// 1000 -> 6.7
	// 10000 -> 9
	// 100,000 -> 11
	mapNameToFood["Felipe"] = "Hay";
	mapNameToFood["Hamza"] = "Pancake";
	mapNameToFood["Dylan"] = "Fish Food";
	mapNameToFood["Ethan"] = "Fish";
	mapNameToFood["Brandon"] = "Pizza";
	mapNameToFood["Brian"] = "Dog food";
	mapNameToFood["Caleb"] = "Eggs";
	mapNameToFood["Christopher"] = "Pie";
	mapNameToFood["David"] = "Cat food";
	mapNameToFood["Dhilip"] = "Noodles";
	mapNameToFood["Harshil"] = "Chocolate";

	std::string favFood = mapNameToFood["Dhilip"];
	std::cout << favFood << std::endl;



	GLFWwindow* window;
	//GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	//GLint mvp_location; /*, vpos_location, vcol_location;*/

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
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

//	pDebugRenderer->RenderDebugObjects(glm::mat4(1.0f), glm::mat4(1.0f), 0.0f);

	// OpenGL and GLFW are good to go, so load the model
	//cModelLoader theModelLoader;   // Stack
	//theModelLoader.LoadPlyModel();


	cModelLoader* pTheModelLoader = new cModelLoader();	// Heap

	cMesh bunnyMesh;		// This is stack based
//	if ( ! pTheModelLoader->LoadPlyModel("assets/models/Sky_Pirate_Combined_xyz.ply", bunnyMesh) )
//	if ( ! pTheModelLoader->LoadPlyModel("assets/models/bun_zipper_res4_XYZ_N.ply", bunnyMesh) )
	if ( ! pTheModelLoader->LoadPlyModel("assets/models/bun_zipper_XYZ_n.ply", bunnyMesh) )
	{
		std::cout << "Didn't find the file" << std::endl;
	}

	cMesh pirateMesh;
	pTheModelLoader->LoadPlyModel("assets/models/Sky_Pirate_Combined_xyz_n.ply", pirateMesh);

	cMesh terrainMesh;
	pTheModelLoader->LoadPlyModel("assets/models/Terrain_XYZ_n.ply", terrainMesh);
//	pTheModelLoader->LoadPlyModel("assets/models/BigFlatTerrain_XYZ_n.ply", terrainMesh);

	cMesh cubeMesh;
	pTheModelLoader->LoadPlyModel("assets/models/Cube_1_Unit_from_origin_XYZ_n.ply", cubeMesh);

	cMesh sphereMesh;
	pTheModelLoader->LoadPlyModel("assets/models/Sphere_Radius_1_XYZ_n.ply", sphereMesh);


	


	cShaderManager* pTheShaderManager = new cShaderManager();

	cShaderManager::cShader vertexShad;
	vertexShad.fileName = "assets/shaders/vertexShader01.glsl";

	cShaderManager::cShader fragShader;
	fragShader.fileName = "assets/shaders/fragmentShader01.glsl";

	if (!pTheShaderManager->createProgramFromFile("SimpleShader", vertexShad, fragShader))
	{
		std::cout << "Error: didn't compile the shader" << std::endl;
		std::cout << pTheShaderManager->getLastError();
		return -1;
	}

	GLuint shaderProgID = pTheShaderManager->getIDFromFriendlyName("SimpleShader");


	//	float x, y, z;		vPosition			"attribute vec3 vPosition;\n"
	//	float r, g, b;		vColour				"attribute vec3 vColour;\n"
//	
//	mvp_location = glGetUniformLocation(program, "MVP");
//	mvp_location = glGetUniformLocation(shaderProgID, "MVP");
	//vpos_location = glGetAttribLocation(program, "vPosition");
	//vcol_location = glGetAttribLocation(program, "vColour");
//
	//glEnableVertexAttribArray(vpos_location);
	//glVertexAttribPointer(vpos_location, 
	//					  3, 
	//					  GL_FLOAT, 
	//					  GL_FALSE,
	//					  sizeof(sVertex),	// sizeof(vertices[0]),
	//					  (void*)0);
//
	//glEnableVertexAttribArray(vcol_location);
	//glVertexAttribPointer(vcol_location, 3, 
	//					  GL_FLOAT, 
	//					  GL_FALSE,
	//					  sizeof(sVertex),	// sizeof(vertices[0]),
	//					  (void*)(sizeof(float) * 3));


	// Create a VAO Manager...
	// #include "cVAOManager.h"  (at the top of your file)
	cVAOManager* pTheVAOManager = new cVAOManager();

	// Note, the "filename" here is really the "model name" 
	//  that we can look up later (i.e. it doesn't have to be the file name)
	sModelDrawInfo drawInfo;
	pTheVAOManager->LoadModelIntoVAO("bunny", 
									 bunnyMesh, 
									 drawInfo, 
									 shaderProgID);

	sModelDrawInfo drawInfoPirate;
	pTheVAOManager->LoadModelIntoVAO("pirate", 
									 pirateMesh,
									 drawInfoPirate, 
									 shaderProgID);

	sModelDrawInfo drawInfoTerrain;
	pTheVAOManager->LoadModelIntoVAO("terrain", 
									 terrainMesh,
									 drawInfoTerrain,
									 shaderProgID);

	sModelDrawInfo cubeMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("cube", 
									 cubeMesh,			// Cube mesh info
									 cubeMeshInfo,
									 shaderProgID);

	sModelDrawInfo sphereMeshInfo;
	pTheVAOManager->LoadModelIntoVAO("sphere", 
									 sphereMesh,		// Sphere mesh info
									 sphereMeshInfo,
									 shaderProgID);


	// At this point, the model is loaded into the GPU


	//// Load up my "scene" 
	//std::vector<cGameObject*> vec_pGameObjects;

	cGameObject* pPirate = new cGameObject();
	pPirate->meshName = "pirate";
	pPirate->friendlyName = "PirateShip";	// Friendly name
	pPirate->positionXYZ = glm::vec3(-30.0f, 20.0f, 10.0f);
	pPirate->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pPirate->scale = 0.75f;
	pPirate->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pPirate->inverseMass = 0.0f;
	pPirate->HACK_AngleAroundYAxis = 0.0f;
	pPirate->HACK_speed = 0.0f;
//
	cGameObject* pBunny = new cGameObject();
	pBunny->meshName = "bunny";
	pBunny->friendlyName = "Bugs";	// Famous bunny
	pBunny->positionXYZ = glm::vec3(50.0f, 20.0f, -2.0f);		// -4 on z
	pBunny->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pBunny->scale = 250.0f;
	pBunny->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	pBunny->inverseMass = 0.0f;
//
	//cGameObject bunny2;
	//bunny2.meshName = "bunny";
	//bunny2.positionXYZ = glm::vec3(0.0f,0.0f,0.0f);
	//bunny2.rotationXYZ = glm::vec3(0.0f,1.0f,0.0f);
	//bunny2.scale = 3.5f;
	//bunny2.objectColourRGBA = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
//
	//cGameObject terrain;
	//terrain.meshName = "terrain";
	//terrain.positionXYZ = glm::vec3(0.0f,-10.0f,0.0f);
	//terrain.rotationXYZ = glm::vec3(0.0f,0.0f,0.0f);
	//terrain.scale = 0.5f;
	//terrain.objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Sphere and cube
	cGameObject* pShpere = new cGameObject();

	//cGameObject A; 
	//cGameObject B;
	//A = B;


	pShpere->meshName = "sphere";
	pShpere->friendlyName = "Sphere#1";	// We use to search 
	pShpere->positionXYZ = glm::vec3(-25.0f, 20.0f, 1.0f);
	pShpere->rotationXYZ = glm::vec3(0.0f,0.0f,0.0f);
	pShpere->scale = 1.0f;
	pShpere->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// Set the sphere's initial velocity, etc.
	pShpere->velocity = glm::vec3(6.0f,-15.0f,0.0f);
	pShpere->accel = glm::vec3(0.0f,0.0f,0.0f);
	pShpere->physicsShapeType = SPHERE;
	pShpere->inverseMass = 1.0f;
//	pShpere->inverseMass = 0.0f;			// Sphere won't move

		// Sphere and cube
	cGameObject* pShpere2 = new cGameObject();
	pShpere2->meshName = "sphere";
	pShpere2->friendlyName = "Sphere#2";
//	pShpere2->positionXYZ = glm::vec3(25.0f, 20.0f, 1.0f);
	pShpere2->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pShpere2->scale = 1.0f;
	pShpere2->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// Set the sphere's initial velocity, etc.
//	pShpere2->velocity = glm::vec3(6.0f, -15.0f, 0.0f);
	pShpere2->accel = glm::vec3(0.0f, 0.0f, 0.0f);
	pShpere2->physicsShapeType = SPHERE;
	pShpere2->inverseMass = 0.0f;
	//	pShpere->inverseMass = 0.0f;			// Sphere won't move


	cGameObject* pCube = new cGameObject();			// HEAP
	pCube->meshName = "cube";
	pCube->positionXYZ = glm::vec3(0.0f, -1.0f, 0.0f);
	pCube->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pCube->scale = 1.0f;
	//pCube->objectColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	pCube->debugColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	pCube->isWireframe = true;
	// Set the sphere's initial velocity, etc.
	//sphere.velocity = glm::vec3(0.0f,0.0f,0.0f);
	//sphere.accel = glm::vec3(0.0f,0.0f,0.0f);
	pCube->inverseMass = 0.0f;	// Ignored during update


	cGameObject* pTerrain = new cGameObject();			// HEAP
	pTerrain->meshName = "terrain";
	pTerrain->friendlyName = "TheGround";
	pTerrain->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pTerrain->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pTerrain->scale = 1.0f;
	pTerrain->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pTerrain->physicsShapeType = MESH;
//	pTerrain->debugColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
//	pTerrain->isWireframe = true;
	pTerrain->inverseMass = 0.0f;	// Ignored during update

	::g_vec_pGameObjects.push_back(pShpere);
	::g_vec_pGameObjects.push_back(pShpere2);
	::g_vec_pGameObjects.push_back(pCube);
	::g_vec_pGameObjects.push_back(pTerrain);
	::g_vec_pGameObjects.push_back(pPirate);
	::g_vec_pGameObjects.push_back(pBunny);



	::g_map_GameObjectsByFriendlyName[pShpere2->friendlyName] = pShpere;
	::g_map_GameObjectsByFriendlyName[pTerrain->friendlyName] = pTerrain;
	::g_map_GameObjectsByFriendlyName[pPirate->friendlyName] = pPirate;
	::g_map_GameObjectsByFriendlyName[pBunny->friendlyName] = pBunny;


	// Will be moved placed around the scene
	cGameObject* pDebugSphere = new cGameObject();
	pDebugSphere->meshName = "sphere";
	pDebugSphere->friendlyName = "debug_sphere";
	pDebugSphere->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pDebugSphere->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pDebugSphere->scale = 0.1f;
//	pDebugSphere->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	pDebugSphere->debugColour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	pDebugSphere->isWireframe = true;
	pDebugSphere->inverseMass = 0.0f;			// Sphere won't move




	//vecGameObjects.push_back(pirate);			// vecGameObjects[0]
	//vecGameObjects.push_back(bunny);		
	//vecGameObjects.push_back(bunny2);
//	vecGameObjects.push_back(terrain);

	//mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);


	glEnable(GL_DEPTH);			// Write to the depth buffer
	glEnable(GL_DEPTH_TEST);	// Test with buffer when drawing

	//float shipVelocityZ = 1.0f;
	//float shipAccelz = 1000.0f;

	
	cPhysics* pPhsyics = new cPhysics();


	cLightHelper* pLightHelper = new cLightHelper();


	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		//       mat4x4 m, p, mvp;
		//glm::mat4 m, p, v, mvp;
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

		//glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0);
		//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

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
		GLint L_0_position = glGetUniformLocation( shaderProgID, "theLights[0].position");
		GLint L_0_diffuse = glGetUniformLocation( shaderProgID, "theLights[0].diffuse");
		GLint L_0_specular = glGetUniformLocation( shaderProgID, "theLights[0].specular");
		GLint L_0_atten = glGetUniformLocation( shaderProgID, "theLights[0].atten");
		GLint L_0_direction = glGetUniformLocation( shaderProgID, "theLights[0].direction");
		GLint L_0_param1 = glGetUniformLocation( shaderProgID, "theLights[0].param1");
		GLint L_0_param2 = glGetUniformLocation( shaderProgID, "theLights[0].param2");

		glUniform4f(L_0_position, 
					sexyLightPosition.x,
					sexyLightPosition.y,
					sexyLightPosition.z,
					1.0f);
		glUniform4f(L_0_diffuse, 1.0f, 1.0f, 1.0f, 1.0f );	// White
		glUniform4f(L_0_specular, 1.0f, 1.0f, 1.0f, 1.0f );	// White
		glUniform4f(L_0_atten, 0.0f,  // constant attenuation
					           sexyLightLinearAtten,  // Linear 
					           sexyLightQuadraticAtten,	// Quadratic 
					           1000000.0f );	// Distance cut off

		// Point light:
		glUniform4f(L_0_param1, 0.0f /*POINT light*/, 0.0f, 0.0f, 1.0f );


		// ********************************************************
		// Move the pirate in the direction it's pointing in...
		
		// Vec4 = mat4x4 * vec4				vertFinal = matModel * vertStart;
		
		cGameObject* pPirate = pFindObjectByFriendlyName("PirateShip");

		glm::mat4 matRotY = glm::rotate(glm::mat4(1.0f),
										pPirate->HACK_AngleAroundYAxis,	//(float)glfwGetTime(),					// Angle 
										glm::vec3(0.0f, 1.0f, 0.0f));
	
		// Assume the ship is at 0,0,0
		glm::vec4 frontOfShip = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);	// pointing to the "front" of the ship
		
		// Vec4 = mat4x4 * vec4				vertFinal = matModel * vertStart;
		glm::vec4 frontOfShipInWorld = matRotY * frontOfShip;

		// this value would be the velocity, ifyou wanted the phsyics update do to it
		glm::vec3 newSpeedOfShipIN_THE_DIRECTION_WE_WANT_TO_GO
			= frontOfShipInWorld * pPirate->HACK_speed;

		// Update the pirate ship
		pPirate->positionXYZ += newSpeedOfShipIN_THE_DIRECTION_WE_WANT_TO_GO;

		// ********************************************************


//	// Point the spot light at the bunny
//	cGameObject* pBunny = pFindObjectByFriendlyName("Bugs");
//	cGameObject* pPirate = pFindObjectByFriendlyName("PirateShip");
//
//	sexyLightPosition = pPirate->positionXYZ;
//
//	// Vector from the bunny to the ship is:
//	// Bunny_Position - ShipPosition;
//	glm::vec3 bunnyToShip = pBunny->positionXYZ - pPirate->positionXYZ;
//	// Normalize the vector (unit or 1.0f length)
//	sexyLightSpotDirection = glm::normalize(bunnyToShip);


		//// Spot light
		//glUniform4f(L_0_direction, 
		//			sexyLightSpotDirection.x,		// 0
		//			sexyLightSpotDirection.y,		// -1
		//			sexyLightSpotDirection.z,		// 0
		//			1.0f );	

		//glUniform4f(L_0_param1,
		//			1.0f /*SPOT light*/, 
		//			sexyLightSpotInnerAngle,		// 15
		//			sexyLightSpotOuterAngle,		// 30
		//			1.0f );


		//glUniform4f(L_0_param1,
		//			2.0f /*DIRECTIONAL light*/, 
		//			sexyLightSpotInnerAngle,		// 15
		//			sexyLightSpotOuterAngle,		// 30
		//			1.0f );


		glUniform4f(L_0_param2, 1.0f /*Light is on*/, 0.0f, 0.0f, 1.0f );

		// Also set the position of my "eye" (the camera)
		//uniform vec4 eyeLocation;
		GLint eyeLocation_UL = glGetUniformLocation( shaderProgID, "eyeLocation");

		glUniform4f( eyeLocation_UL, 
					 cameraEye.x, cameraEye.y, cameraEye.z, 1.0f );


		std::stringstream ssTitle;
		ssTitle 
			<< sexyLightPosition.x << ", " 
			<< sexyLightPosition.y << ", " 
			<< sexyLightPosition.z 
			<< "Atten: "
			<< sexyLightConstAtten << " : "
			<< sexyLightLinearAtten << " : "
			<< sexyLightQuadraticAtten;
		glfwSetWindowTitle( window, ssTitle.str().c_str() );


		GLint matView_UL = glGetUniformLocation(shaderProgID, "matView");
		GLint matProj_UL = glGetUniformLocation(shaderProgID, "matProj");

		glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));


		// **************************************************
		// **************************************************
		// Loop to draw everything in the scene


		for (int index = 0; index != ::g_vec_pGameObjects.size(); index++)
		{
			glm::mat4 matModel = glm::mat4(1.0f);

			cGameObject* pCurrentObject = ::g_vec_pGameObjects[index];

			DrawObject( matModel, pCurrentObject, 
					   shaderProgID, pTheVAOManager);

		}//for (int index...


		// Update the objects through physics
//		PhysicsUpdate( vec_pGameObjects, 0.01f );

		pPhsyics->IntegrationStep(::g_vec_pGameObjects, 0.01f);

		// Let's draw all the closest points to the sphere
		// on the terrain mesh....
		// 
		// For each triangle in the terrain mesh:
		// - Run ClosestPointToTriangle
		// - Place the debug sphere "there"
		// - Draw it.

		glm::vec3 closestPoint = glm::vec3(0.0f,0.0f,0.0f);
		cPhysics::sPhysicsTriangle closestTriangle;

		pPhsyics->GetClosestTriangleToPoint(pShpere->positionXYZ, terrainMesh, closestPoint, closestTriangle);

		bool DidBallCollideWithGround = false;
		HACK_BounceOffSomePlanes(pShpere, DidBallCollideWithGround );

		// A more general 
		pPhsyics->TestForCollisions(::g_vec_pGameObjects);


//		float closestDistanceSoFar = FLT_MAX;
//		glm::vec3 closetPoint = glm::vec3(0.0f,0.0f,0.0f);
//		sPlyTriangle closetTriangle;
//
//		for (unsigned int triIndex = 0;
//			 triIndex != terrainMesh.vecTriangles.size();
//			 triIndex++)
//		{
//			sPlyTriangle& curTriangle = terrainMesh.vecTriangles[triIndex];
//
//			// Get the vertices of the triangle
//			sPlyVertexXYZ_N triVert1 = terrainMesh.vecVertices[curTriangle.vert_index_1];
//			sPlyVertexXYZ_N triVert2 = terrainMesh.vecVertices[curTriangle.vert_index_2];
//			sPlyVertexXYZ_N triVert3 = terrainMesh.vecVertices[curTriangle.vert_index_3];
//
//			Point triVertPoint1;
//			triVertPoint1.x = triVert1.x;
//			triVertPoint1.y = triVert1.y;
//			triVertPoint1.z = triVert1.z;
//
//			Point triVertPoint2;
//			triVertPoint2.x = triVert2.x;
//			triVertPoint2.y = triVert2.y;
//			triVertPoint2.z = triVert2.z;
//
//			Point triVertPoint3;
//			triVertPoint3.x = triVert3.x;
//			triVertPoint3.y = triVert3.y;
//			triVertPoint3.z = triVert3.z;
//
//			glm::vec3 curClosetPoint = ClosestPtPointTriangle( 
//				pShpere->positionXYZ, 
//				triVertPoint1, triVertPoint2, triVertPoint3 );
//			
//			// Is this the closest so far?
//			float distanceNow = glm::distance(curClosetPoint, pShpere->positionXYZ);
//
//			// is this closer than the closest distance
//			if ( distanceNow <= closestDistanceSoFar )
//			{
//				closestDistanceSoFar = distanceNow;
//				closetPoint = curClosetPoint;
//			}
//
//			//glm::mat4 matModel = glm::mat4(1.0f);
//			//pDebugSphere->positionXYZ = closetPoint;
//			//DrawObject(matModel, pDebugSphere, 
//			//			   shaderProgID, pTheVAOManager);
//
//
//		}//for (unsigned int triIndex = 0;

		{// Draw closest point
			glm::mat4 matModel = glm::mat4(1.0f);
			pDebugSphere->positionXYZ = closestPoint;
			pDebugSphere->scale = 1.0f;
			pDebugSphere->debugColour = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
			pDebugSphere->isWireframe = true;
			DrawObject(matModel, pDebugSphere,
					   shaderProgID, pTheVAOManager);
		}


		// How far did we penetrate the surface?
		glm::vec3 CentreToClosestPoint = pShpere->positionXYZ - closestPoint;

		// Direction that ball is going is normalized velocity
		glm::vec3 directionBall = glm::normalize(pShpere->velocity);	// 1.0f

		// Calcualte direction to move it back the way it came from
		glm::vec3 oppositeDirection = -directionBall;				// 1.0f

		float distanceToClosestPoint = glm::length(CentreToClosestPoint);




		// HACK
		if (DidBallCollideWithGround)
		{ 
			float sphereRadius = 1.0f;
			float distanceToMoveBack = sphereRadius - distanceToClosestPoint;

			glm::vec3 adjustmentVector = oppositeDirection * distanceToMoveBack;

			// Let's move the sphere that amount...
			pShpere->positionXYZ += adjustmentVector;


			// NOW, I can calculate the correct response vector... 

//			pShpere->velocity = glm::reflect(pShpere->velocity, triangleNormal)
		}


		std::cout 
			<< pShpere->velocity.x << ", "
			<< pShpere->velocity.y << ", "
			<< pShpere->velocity.z << "   dist = "
			<< distanceToClosestPoint << std::endl;

		//howMuchToMoveItBack = 1.0 - lenthOfThatVector


		if (bLightDebugSheresOn) 
		{
			{// Draw where the light is at
				glm::mat4 matModel = glm::mat4(1.0f);
				pDebugSphere->positionXYZ = sexyLightPosition;
				pDebugSphere->scale = 0.5f;
				pDebugSphere->debugColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
				pDebugSphere->isWireframe = true;
				DrawObject(matModel, pDebugSphere,
						   shaderProgID, pTheVAOManager);
			}

			// Draw spheres to represent the attenuation...
			{   // Draw a sphere at 1% brightness
				glm::mat4 matModel = glm::mat4(1.0f);
				pDebugSphere->positionXYZ = sexyLightPosition;
				float sphereSize = pLightHelper->calcApproxDistFromAtten( 
													   0.01f,		// 1% brightness (essentially black)
													   0.001f,		// Within 0.1%  
													   100000.0f,	// Will quit when it's at this distance
													   sexyLightConstAtten,
													   sexyLightLinearAtten,
													   sexyLightQuadraticAtten );
				pDebugSphere->scale = sphereSize;
				pDebugSphere->debugColour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
				pDebugSphere->isWireframe = true;
				DrawObject(matModel, pDebugSphere,
						   shaderProgID, pTheVAOManager);
			}
			{   // Draw a sphere at 25% brightness
				glm::mat4 matModel = glm::mat4(1.0f);
				pDebugSphere->positionXYZ = sexyLightPosition;
				float sphereSize = pLightHelper->calcApproxDistFromAtten( 
													   0.25f,		// 1% brightness (essentially black)
													   0.001f,		// Within 0.1%  
													   100000.0f,	// Will quit when it's at this distance
													   sexyLightConstAtten,
													   sexyLightLinearAtten,
													   sexyLightQuadraticAtten );
				pDebugSphere->scale = sphereSize;
				pDebugSphere->debugColour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
				pDebugSphere->isWireframe = true;
				DrawObject(matModel, pDebugSphere,
						   shaderProgID, pTheVAOManager);
			}
			{   // Draw a sphere at 50% brightness
				glm::mat4 matModel = glm::mat4(1.0f);
				pDebugSphere->positionXYZ = sexyLightPosition;
				float sphereSize = pLightHelper->calcApproxDistFromAtten( 
													   0.50f,		// 1% brightness (essentially black)
													   0.001f,		// Within 0.1%  
													   100000.0f,	// Will quit when it's at this distance
													   sexyLightConstAtten,
													   sexyLightLinearAtten,
													   sexyLightQuadraticAtten );
				pDebugSphere->scale = sphereSize;
				pDebugSphere->debugColour = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
				pDebugSphere->isWireframe = true;
				DrawObject(matModel, pDebugSphere,
						   shaderProgID, pTheVAOManager);
			}
			{   // Draw a sphere at 75% brightness
				glm::mat4 matModel = glm::mat4(1.0f);
				pDebugSphere->positionXYZ = sexyLightPosition;
				float sphereSize = pLightHelper->calcApproxDistFromAtten( 
													   0.75f,		// 1% brightness (essentially black)
													   0.001f,		// Within 0.1%  
													   100000.0f,	// Will quit when it's at this distance
													   sexyLightConstAtten,
													   sexyLightLinearAtten,
													   sexyLightQuadraticAtten );
				pDebugSphere->scale = sphereSize;
				pDebugSphere->debugColour = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
				pDebugSphere->isWireframe = true;
				DrawObject(matModel, pDebugSphere,
						   shaderProgID, pTheVAOManager);
			}
			{   // Draw a sphere at 95% brightness
				glm::mat4 matModel = glm::mat4(1.0f);
				pDebugSphere->positionXYZ = sexyLightPosition;
				float sphereSize = pLightHelper->calcApproxDistFromAtten( 
													   0.95f,		// 1% brightness (essentially black)
													   0.001f,		// Within 0.1%  
													   100000.0f,	// Will quit when it's at this distance
													   sexyLightConstAtten,
													   sexyLightLinearAtten,
													   sexyLightQuadraticAtten );
				pDebugSphere->scale = sphereSize;
				pDebugSphere->debugColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
				pDebugSphere->isWireframe = true;
				DrawObject(matModel, pDebugSphere,
						   shaderProgID, pTheVAOManager);
			}
		}// if (bLightDebugSheresOn) 

		 // **************************************************
		// **************************************************




		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();


	// Delete everything
	delete pTheModelLoader;
//	delete pTheVAOManager;

	// Watch out!!
	// sVertex* pVertices = new sVertex[numberOfVertsOnGPU];
//	delete [] pVertices;		// If it's an array, also use [] bracket

	exit(EXIT_SUCCESS);
}


void DrawObject(glm::mat4 m, 
				cGameObject* pCurrentObject, 
				GLint shaderProgID,
				cVAOManager* pVAOManager)
{
	// 
				//         mat4x4_identity(m);
	m = glm::mat4(1.0f);



	// ******* TRANSLATION TRANSFORM *********
	glm::mat4 matTrans
	= glm::translate(glm::mat4(1.0f),
					 glm::vec3(pCurrentObject->positionXYZ.x,
							   pCurrentObject->positionXYZ.y,
							   pCurrentObject->positionXYZ.z));
	m = m * matTrans;
	// ******* TRANSLATION TRANSFORM *********



	// ******* ROTATION TRANSFORM *********
	//mat4x4_rotate_Z(m, m, (float) glfwGetTime());
	glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
									pCurrentObject->rotationXYZ.z,					// Angle 
									glm::vec3(0.0f, 0.0f, 1.0f));
	m = m * rotateZ;

	glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
									pCurrentObject->rotationXYZ.y,	//(float)glfwGetTime(),					// Angle 
									glm::vec3(0.0f, 1.0f, 0.0f));
	m = m * rotateY;

	glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
									pCurrentObject->rotationXYZ.x,	// (float)glfwGetTime(),					// Angle 
									glm::vec3(1.0f, 0.0f, 0.0f));
	m = m * rotateX;
	// ******* ROTATION TRANSFORM *********



	// ******* SCALE TRANSFORM *********
	glm::mat4 scale = glm::scale(glm::mat4(1.0f),
								 glm::vec3(pCurrentObject->scale,
										   pCurrentObject->scale,
										   pCurrentObject->scale));
	m = m * scale;
	// ******* SCALE TRANSFORM *********



	//mat4x4_mul(mvp, p, m);
	//mvp = p * v * m;

	// Choose which shader to use
	//glUseProgram(program);
	glUseProgram(shaderProgID);


	//glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
	//glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

	//uniform mat4 matModel;		// Model or World 
	//uniform mat4 matView; 		// View or camera
	//uniform mat4 matProj;
	GLint matModel_UL = glGetUniformLocation(shaderProgID, "matModel");

	glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(m));
	//glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
	//glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));



	// Find the location of the uniform variable newColour
	GLint newColour_location = glGetUniformLocation(shaderProgID, "newColour");

	glUniform3f(newColour_location,
				pCurrentObject->objectColourRGBA.r,
				pCurrentObject->objectColourRGBA.g,
				pCurrentObject->objectColourRGBA.b);

	//uniform float newColourRed;
	//uniform float newColourGreen;
	//uniform float newColourBlue;
	//GLint newColourRed_UL = glGetUniformLocation(shaderProgID, "newColourRed");
	//GLint newColourGreen_UL = glGetUniformLocation(shaderProgID, "newColourGreen");
	//GLint newColourBlue_UL = glGetUniformLocation(shaderProgID, "newColourBlue");

	//glUniform1f(newColourRed_UL, pCurrentObject->objectColourRGBA.r);
	//glUniform1f(newColourGreen_UL, pCurrentObject->objectColourRGBA.g);
	//glUniform1f(newColourBlue_UL, pCurrentObject->objectColourRGBA.b);
	//GLint lighPosition_UL = glGetUniformLocation(shaderProgID, "lightPosition");
	//glUniform3f(lighPosition_UL, sexyLightLocation.x,
	//			sexyLightLocation.y, sexyLightLocation.z);

	GLint diffuseColour_UL = glGetUniformLocation(shaderProgID, "diffuseColour");
	glUniform4f(diffuseColour_UL, 
				pCurrentObject->objectColourRGBA.r,
				pCurrentObject->objectColourRGBA.g,
				pCurrentObject->objectColourRGBA.b,
				pCurrentObject->objectColourRGBA.a);	// 

	GLint specularColour_UL = glGetUniformLocation(shaderProgID, "specularColour");
	glUniform4f(specularColour_UL,
				1.0f,	// R
				1.0f,	// G
				1.0f,	// B
				1000.0f);	// Specular "power" (how shinny the object is)
	                        // 1.0 to really big (10000.0f)


	//uniform vec4 debugColour;
	//uniform bool bDoNotLight;
	GLint debugColour_UL = glGetUniformLocation(shaderProgID, "debugColour");
	GLint bDoNotLight_UL = glGetUniformLocation(shaderProgID, "bDoNotLight");

	if ( pCurrentObject->isWireframe )
	{ 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// LINES
		glUniform4f( debugColour_UL, 
					pCurrentObject->debugColour.r,
					pCurrentObject->debugColour.g,
					pCurrentObject->debugColour.b,
					pCurrentObject->debugColour.a);
		glUniform1f(bDoNotLight_UL, (float)GL_TRUE);
	}
	else
	{	// Regular object (lit and not wireframe)
		glUniform1f(bDoNotLight_UL, (float)GL_FALSE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
	}
	//glPointSize(15.0f);

	if (pCurrentObject->disableDepthBufferTest)
	{
		glDisable(GL_DEPTH_TEST);					// DEPTH Test OFF
	}
	else
	{
		glEnable(GL_DEPTH_TEST);						// Turn ON depth test
	}

	if (pCurrentObject->disableDepthBufferWrite)
	{
		glDisable(GL_DEPTH);						// DON'T Write to depth buffer
	}
	else
	{
		glEnable(GL_DEPTH);								// Write to depth buffer
	}


	//		glDrawArrays(GL_TRIANGLES, 0, 2844);
	//		glDrawArrays(GL_TRIANGLES, 0, numberOfVertsOnGPU);

	sModelDrawInfo drawInfo;
	//if (pTheVAOManager->FindDrawInfoByModelName("bunny", drawInfo))
	if (pVAOManager->FindDrawInfoByModelName(pCurrentObject->meshName, drawInfo))
	{
		glBindVertexArray(drawInfo.VAO_ID);
		glDrawElements(GL_TRIANGLES,
					   drawInfo.numberOfIndices,
					   GL_UNSIGNED_INT,
					   0);
		glBindVertexArray(0);
	}
	
	return;
} // DrawObject;
// 

// returns NULL (0) if we didn't find it.
cGameObject* pFindObjectByFriendlyName(std::string name)
{
	// Do a linear search 
	for (unsigned int index = 0;
		 index != g_vec_pGameObjects.size(); index++)
	{
		if (::g_vec_pGameObjects[index]->friendlyName == name)
		{
			// Found it!!
			return ::g_vec_pGameObjects[index];
		}
	}
	// Didn't find it
	return NULL;
}

// returns NULL (0) if we didn't find it.
cGameObject* pFindObjectByFriendlyNameMap(std::string name)
{
	//std::map<std::string, cGameObject*> g_map_GameObjectsByFriendlyName;
	return ::g_map_GameObjectsByFriendlyName[name];
}