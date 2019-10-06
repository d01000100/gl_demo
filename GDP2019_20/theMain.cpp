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
bool bLightDebugSheresOn = false;

// Load up my "scene"  (now global)
std::vector<cGameObject*> g_vec_pGameObjects;
std::map<std::string /*FriendlyName*/, cGameObject*> g_map_GameObjectsByFriendlyName;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Move the sphere to where the camera is and shoot the ball from there...

	cGameObject* pTheBall = pFindObjectByFriendlyName("Sphere#1");

	// What's the velocity
	// Target - eye = direction
	glm::vec3 direction = glm::normalize( cameraTarget - cameraEye ); 

	float speed = 10.0f; 

	pTheBall->velocity = direction * speed;
	pTheBall->positionXYZ = cameraEye;

	return;
}

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
	//glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	cDebugRenderer* pDebugRenderer = new cDebugRenderer();
	pDebugRenderer->initialize();

	cModelLoader* pTheModelLoader = new cModelLoader();	// Heap

	cMesh largeBunnyMesh;
	pTheModelLoader->LoadPlyModel("assets/models/Large_Physics_Bunny_XYZ_N.ply", largeBunnyMesh);

	cMesh terrainMesh;
	pTheModelLoader->LoadPlyModel("assets/models/Terrain_XYZ_n.ply", terrainMesh);

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

	// Create a VAO Manager...
	cVAOManager* pTheVAOManager = new cVAOManager();

	sModelDrawInfo drawInfoTerrain;
	pTheVAOManager->LoadModelIntoVAO("terrain", 
									 terrainMesh,
									 drawInfoTerrain,
									 shaderProgID);

	sModelDrawInfo largeBunnyDrawInfo;
	pTheVAOManager->LoadModelIntoVAO("large_bunny", 
									 largeBunnyMesh,		// Sphere mesh info
									 largeBunnyDrawInfo,
									 shaderProgID);

	cGameObject* pTerrain = new cGameObject();			// HEAP
	pTerrain->meshName = "terrain";
	pTerrain->friendlyName = "TheGround";
	pTerrain->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pTerrain->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pTerrain->scale = 1.0f;
	pTerrain->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pTerrain->physicsShapeType = MESH;
	pTerrain->inverseMass = 0.0f;	// Ignored during update
	pTerrain->isVisible = false;

	cGameObject* pLargeBunny = new cGameObject();			// HEAP
	pLargeBunny->meshName = "large_bunny";
	pLargeBunny->friendlyName = "largeBunny";
	pLargeBunny->positionXYZ = glm::vec3(0.0f, 20.0f, 0.0f);
	pLargeBunny->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pLargeBunny->scale = 1.0f;	//***** SCALE = 1.0f *****/
	pLargeBunny->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pLargeBunny->physicsShapeType = MESH;
	pLargeBunny->inverseMass = 0.0f;	// Ignored during update

	::g_vec_pGameObjects.push_back(pTerrain);
	::g_vec_pGameObjects.push_back(pLargeBunny);

	// Will be moved placed around the scene
	cGameObject* pDebugSphere = new cGameObject();
	pDebugSphere->meshName = "sphere";
	pDebugSphere->friendlyName = "debug_sphere";
	pDebugSphere->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pDebugSphere->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pDebugSphere->scale = 0.1f;
	pDebugSphere->debugColour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	pDebugSphere->isWireframe = true;
	pDebugSphere->inverseMass = 0.0f;			// Sphere won't move

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
		
		glUniform4f(L_0_param2, 1.0f /*Light is on*/, 0.0f, 0.0f, 1.0f );

		// Also set the position of my "eye" (the camera)
		//uniform vec4 eyeLocation;
		GLint eyeLocation_UL = glGetUniformLocation( shaderProgID, "eyeLocation");

		glUniform4f( eyeLocation_UL, 
					 cameraEye.x, cameraEye.y, cameraEye.z, 1.0f );


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

		double averageDeltaTime = avgDeltaTimeThingy.getAverage();
		pPhsyics->IntegrationStep(::g_vec_pGameObjects, (float)averageDeltaTime);
		
		pDebugRenderer->RenderDebugObjects( v, p, 0.01f );
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	// Delete everything
	delete pTheModelLoader;

	exit(EXIT_SUCCESS);
}


void DrawObject(glm::mat4 m, 
				cGameObject* pCurrentObject, 
				GLint shaderProgID,
				cVAOManager* pVAOManager)
{
	m = glm::mat4(1.0f);

	// ******* TRANSLATION TRANSFORM *********
	glm::mat4 matTrans
	= glm::translate(glm::mat4(1.0f),
					 glm::vec3(pCurrentObject->positionXYZ.x,
							   pCurrentObject->positionXYZ.y,
							   pCurrentObject->positionXYZ.z));
	m = m * matTrans;

	// ******* ROTATION TRANSFORM *********
	glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
									pCurrentObject->rotationXYZ.z,
									glm::vec3(0.0f, 0.0f, 1.0f));
	m = m * rotateZ;

	glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
									pCurrentObject->rotationXYZ.y,
									glm::vec3(0.0f, 1.0f, 0.0f));
	m = m * rotateY;

	glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
									pCurrentObject->rotationXYZ.x,
									glm::vec3(1.0f, 0.0f, 0.0f));
	m = m * rotateX;

	// ******* SCALE TRANSFORM *********
	glm::mat4 scale = glm::scale(glm::mat4(1.0f),
								 glm::vec3(pCurrentObject->scale,
										   pCurrentObject->scale,
										   pCurrentObject->scale));
	m = m * scale;

	//uniform mat4 matModel;		// Model or World 
	//uniform mat4 matView; 		// View or camera
	//uniform mat4 matProj;
	GLint matModel_UL = glGetUniformLocation(shaderProgID, "matModel");

	glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(m));

	// Calcualte the inverse transpose of the model matrix and pass that...
	// Stripping away scaling and translation, leaving only rotation
	// Because the normal is only a direction, really
	GLint matModelIT_UL = glGetUniformLocation(shaderProgID, "matModelInverseTranspose");
	glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(m));
	glUniformMatrix4fv(matModelIT_UL, 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));

	// Find the location of the uniform variable newColour
	GLint newColour_location = glGetUniformLocation(shaderProgID, "newColour");

	glUniform3f(newColour_location,
				pCurrentObject->objectColourRGBA.r,
				pCurrentObject->objectColourRGBA.g,
				pCurrentObject->objectColourRGBA.b);

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

	sModelDrawInfo drawInfo;
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
	return ::g_map_GameObjectsByFriendlyName[name];
}