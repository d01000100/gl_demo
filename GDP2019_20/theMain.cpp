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

#include "cModelLoader.h"			
#include "cVAOManager.h"		// NEW
#include "cGameObject.h"

#include "cShaderManager.h"

#include <sstream>

#include <limits.h>
#include <float.h>

// The Physics function
#include "PhysicsStuff.h"


void DrawObject(glm::mat4 m,
				cGameObject* pCurrentObject,
				GLint shaderProgID,
				cVAOManager* pVAOManager);


glm::vec3 cameraEye = glm::vec3(0.0, 80.0, -80.0);
glm::vec3 cameraTarget = glm::vec3(0.0f, 8.0, 0.0f);
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 sexyLightLocation = glm::vec3(0.0f,0.0f,0.0f);


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	const float CAMERASPEED = 10.0f;

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


	if (key == GLFW_KEY_J)
	{
		sexyLightLocation.x -= CAMERASPEED;
	}
	if (key == GLFW_KEY_K)
	{
		sexyLightLocation.x += CAMERASPEED;
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



	GLFWwindow* window;
	//GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location; /*, vpos_location, vcol_location;*/

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


	// OpenGL and GLFW are good to go, so load the model
	//cModelLoader theModelLoader;   // Stack
	//theModelLoader.LoadPlyModel();


	cModelLoader* pTheModelLoader = new cModelLoader();	// Heap

	cMesh bunnyMesh;		// This is stack based
//	if ( ! pTheModelLoader->LoadPlyModel("assets/models/Sky_Pirate_Combined_xyz.ply", bunnyMesh) )
	if ( ! pTheModelLoader->LoadPlyModel("assets/models/bun_zipper_res4_XYZ.ply", bunnyMesh) )
	{
		std::cout << "Didn't find the file" << std::endl;
	}

	cMesh pirateMesh;
	pTheModelLoader->LoadPlyModel("assets/models/Sky_Pirate_Combined_xyz.ply", pirateMesh);

	cMesh terrainMesh;
//	pTheModelLoader->LoadPlyModel("assets/models/Terrain.ply", terrainMesh);
	pTheModelLoader->LoadPlyModel("assets/models/BigFlatTerrain.ply", terrainMesh);

	cMesh cubeMesh;
	pTheModelLoader->LoadPlyModel("assets/models/Cube_1_Unit_from_origin_XYZ.ply", cubeMesh);

	cMesh sphereMesh;
	pTheModelLoader->LoadPlyModel("assets/models/Sphere_Radius_1_XYZ.ply", sphereMesh);

	// **
	// At this point, our model is loaded and stored into a cMesh object.
	// Pass this cMesh object into the VAOManager to put onto the GPU
	// **
//
	// Copy the mesh into a local array to go to the GPU
	//sVertex* pVertices = NULL;
//
	//unsigned int numberOfVertsOnGPU = bunnyMesh.vecTriangles.size() * 3;
	//// Dynamic allocation of memory...
	//pVertices = new sVertex[numberOfVertsOnGPU];
//
	//// Copy the data from the cMesh format to the sVertex array format
	//// { -0.0248608, 0.122913,  3.0f,  1.0f, 1.0f, 1.0f },
//
	//unsigned int triIndex = 0;	// Index into the cMesh triangle array
	//unsigned int vertIndex = 0;	// Index into the vertex array 
	//for (; triIndex != bunnyMesh.vecTriangles.size(); triIndex++, vertIndex += 3)
	//{
	//	// Make a copy (so that the next line is not crazy long)
	//	sPlyTriangle tempVert = bunnyMesh.vecTriangles[triIndex];
//
	//	// The one for the GPU (vertex buffer)
	//	pVertices[vertIndex + 0].x = bunnyMesh.vecVertices[tempVert.vert_index_1].x;
	//	pVertices[vertIndex + 0].y = bunnyMesh.vecVertices[tempVert.vert_index_1].y;
	//	pVertices[vertIndex + 0].z = bunnyMesh.vecVertices[tempVert.vert_index_1].z;
	//	pVertices[vertIndex + 0].r = 1.0f;
	//	pVertices[vertIndex + 0].g = 1.0f;
	//	pVertices[vertIndex + 0].b = 1.0f;
//
	//	pVertices[vertIndex + 1].x = bunnyMesh.vecVertices[tempVert.vert_index_2].x;
	//	pVertices[vertIndex + 1].y = bunnyMesh.vecVertices[tempVert.vert_index_2].y;
	//	pVertices[vertIndex + 1].z = bunnyMesh.vecVertices[tempVert.vert_index_2].z;
	//	pVertices[vertIndex + 1].r = 1.0f;
	//	pVertices[vertIndex + 1].g = 1.0f;
	//	pVertices[vertIndex + 1].b = 1.0f;
//
	//	pVertices[vertIndex + 2].x = bunnyMesh.vecVertices[tempVert.vert_index_3].x;
	//	pVertices[vertIndex + 2].y = bunnyMesh.vecVertices[tempVert.vert_index_3].y;
	//	pVertices[vertIndex + 2].z = bunnyMesh.vecVertices[tempVert.vert_index_3].z;
	//	pVertices[vertIndex + 2].r = 1.0f;
	//	pVertices[vertIndex + 2].g = 1.0f;
	//	pVertices[vertIndex + 2].b = 1.0f;
//
//
//
	//}// for (; triIndex !=
//
//
	//// NOTE: OpenGL error checks have been omitted for brevity
	//glGenBuffers(1, &vertex_buffer);
	//glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//
	//unsigned int sizeOfVertexBufferInBytes = 
	//	numberOfVertsOnGPU * sizeof(sVertex);
//
	//// Copies the data into the CURRENT buffer
	//glBufferData(GL_ARRAY_BUFFER, 
	//			 sizeOfVertexBufferInBytes, // sizeof(vertices), 
	//			 pVertices,					//void*  vertices, 
	//			 GL_STATIC_DRAW);
//
	////sVertex* pVertices = NULL;
	////sVertex vertices[4822] = {....}
	


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
	mvp_location = glGetUniformLocation(shaderProgID, "MVP");
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


	// Load up my "scene" 
	std::vector<cGameObject*> vec_pGameObjects;

	//cGameObject pirate;
	//pirate.meshName = "pirate";
	//pirate.positionXYZ = glm::vec3(0.0f, 0.0f, 10.0f);
	//pirate.rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	//pirate.scale = 0.75f;
	//pirate.objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//
	//cGameObject bunny;
	//bunny.meshName = "bunny";
	//bunny.positionXYZ = glm::vec3(0.0f, 0.0f, -2.0f);		// -4 on z
	//bunny.rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	//bunny.scale = 5.0f;
	//bunny.objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
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
	pShpere->meshName = "sphere";
	pShpere->positionXYZ = glm::vec3(-100.0f, 100.0f, 1.0f);
	pShpere->rotationXYZ = glm::vec3(0.0f,0.0f,0.0f);
	pShpere->scale = 1.0f;
	pShpere->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// Set the sphere's initial velocity, etc.
	pShpere->velocity = glm::vec3(6.0f,0.0f,0.0f);
	pShpere->accel = glm::vec3(0.0f,0.0f,0.0f);
	pShpere->inverseMass = 1.0f;
//	pShpere->inverseMass = 0.0f;			// Sphere won't move


	cGameObject* pCube = new cGameObject();			// HEAP
	pCube->meshName = "cube";
	pCube->positionXYZ = glm::vec3(0.0f, -1.0f, 0.0f);
	pCube->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pCube->scale = 1.0f;
	pCube->objectColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	// Set the sphere's initial velocity, etc.
	//sphere.velocity = glm::vec3(0.0f,0.0f,0.0f);
	//sphere.accel = glm::vec3(0.0f,0.0f,0.0f);
	pCube->inverseMass = 0.0f;	// Ignored during update


	cGameObject* pTerrain = new cGameObject();			// HEAP
	pTerrain->meshName = "terrain";
	pTerrain->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pTerrain->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pTerrain->scale = 1.0f;
	pTerrain->objectColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	pTerrain->inverseMass = 0.0f;	// Ignored during update

	vec_pGameObjects.push_back(pShpere);
	vec_pGameObjects.push_back(pCube);
	vec_pGameObjects.push_back(pTerrain);


	// Will be moved placed around the scene
	cGameObject* pDebugSphere = new cGameObject();
	pDebugSphere->meshName = "sphere";
	pDebugSphere->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pDebugSphere->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pDebugSphere->scale = 0.1f;
	pDebugSphere->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
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


	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		//       mat4x4 m, p, mvp;
		glm::mat4 m, p, v, mvp;

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


		// 
		//vecGameObjects[0].positionXYZ.x -= 0.001f;
		//vecGameObjects[1].rotationXYZ.y += 0.001f;
		//vecGameObjects[1].scale += 0.01f;


		// Move the pirate ship
	// for the physics floating point example

		//const float DELTATIME = 1.0f / 60.f;
		//vecGameObjects[0].positionXYZ.z += shipVelocityZ * DELTATIME;

		//// Accel
		//shipVelocityZ += shipAccelz * DELTATIME;


//		cameraEye.z = vecGameObjects[0].positionXYZ.z - 20.0f;
//		cameraTarget = vecGameObjects[0].positionXYZ;


//		std::stringstream ssTitle;
//		ssTitle << vecGameObjects[0].positionXYZ.z;
//		glfwSetWindowTitle( window, ssTitle.str().c_str() );

		// Update the objects through physics
		PhysicsUpdate( vec_pGameObjects, 0.01f );


		GLint matView_UL = glGetUniformLocation(shaderProgID, "matView");
		GLint matProj_UL = glGetUniformLocation(shaderProgID, "matProj");

		glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));


		// **************************************************
		// **************************************************
		// Loop to draw everything in the scene
		for (int index = 0; index != vec_pGameObjects.size(); index++)
		{
			glm::mat4 matModel = glm::mat4(1.0f);

			cGameObject* pCurrentObject = vec_pGameObjects[index];

			DrawObject( matModel, pCurrentObject, 
					   shaderProgID, pTheVAOManager);

		}//for (int index...


		// Let's draw all the closest points to the sphere
		// on the terrain mesh....
		// 
		// For each triangle in the terrain mesh:
		// - Run ClosestPointToTriangle
		// - Place the debug sphere "there"
		// - Draw it.

		float closestDistanceSoFar = FLT_MAX;
		glm::vec3 closetPoint = glm::vec3(0.0f,0.0f,0.0f);

		for (unsigned int triIndex = 0;
			 triIndex != terrainMesh.vecTriangles.size();
			 triIndex++)
		{
			sPlyTriangle& curTriangle = terrainMesh.vecTriangles[triIndex];

			// Get the vertices of the triangle
			sPlyVertexXYZ triVert1 = terrainMesh.vecVertices[curTriangle.vert_index_1];
			sPlyVertexXYZ triVert2 = terrainMesh.vecVertices[curTriangle.vert_index_2];
			sPlyVertexXYZ triVert3 = terrainMesh.vecVertices[curTriangle.vert_index_3];

			Point triVertPoint1;
			triVertPoint1.x = triVert1.x;
			triVertPoint1.y = triVert1.y;
			triVertPoint1.z = triVert1.z;

			Point triVertPoint2;
			triVertPoint2.x = triVert2.x;
			triVertPoint2.y = triVert2.y;
			triVertPoint2.z = triVert2.z;

			Point triVertPoint3;
			triVertPoint3.x = triVert3.x;
			triVertPoint3.y = triVert3.y;
			triVertPoint3.z = triVert3.z;

			glm::vec3 curClosetPoint = ClosestPtPointTriangle( 
				pShpere->positionXYZ, 
				triVertPoint1, triVertPoint2, triVertPoint3 );
			
			// Is this the closest so far?
			float distanceNow = glm::distance(curClosetPoint, pShpere->positionXYZ);

			// is this closer than the closest distance
			if ( distanceNow <= closestDistanceSoFar )
			{
				closestDistanceSoFar = distanceNow;
				closetPoint = curClosetPoint;
			}

			//glm::mat4 matModel = glm::mat4(1.0f);
			//pDebugSphere->positionXYZ = closetPoint;
			//DrawObject(matModel, pDebugSphere, 
			//			   shaderProgID, pTheVAOManager);


		}//for (unsigned int triIndex = 0;

		glm::mat4 matModel = glm::mat4(1.0f);
		pDebugSphere->positionXYZ = closetPoint;
		DrawObject(matModel, pDebugSphere,
				   shaderProgID, pTheVAOManager);



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
	GLint newColourRed_UL = glGetUniformLocation(shaderProgID, "newColourRed");
	GLint newColourGreen_UL = glGetUniformLocation(shaderProgID, "newColourGreen");
	GLint newColourBlue_UL = glGetUniformLocation(shaderProgID, "newColourBlue");

	glUniform1f(newColourRed_UL, pCurrentObject->objectColourRGBA.r);
	glUniform1f(newColourGreen_UL, pCurrentObject->objectColourRGBA.g);
	glUniform1f(newColourBlue_UL, pCurrentObject->objectColourRGBA.b);


	GLint lighPosition_UL = glGetUniformLocation(shaderProgID, "lightPosition");
	glUniform3f(lighPosition_UL, sexyLightLocation.x,
				sexyLightLocation.y, sexyLightLocation.z);



	// This will change the fill mode to something 
	//  GL_FILL is solid 
	//  GL_LINE is "wireframe"
	//glPointSize(15.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


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