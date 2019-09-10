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

// Is already inside the cVAOManager.h file
//struct sVertex
//{
//	float x, y, z;			// 32 bit 4 bytes  12
//	float r, g, b;			// 12 bytes
//};

//sVertex vertices[5000];		//	on the stack
//sVertex* pVertices = new sVertex[5000];			// On the heap
////sVertex vertices[1] =
//{
//	{ -0.0248608, 0.122913, 0.0245429,  1.0f, 1.0f, 1.0f },
//
//};
//sVertex vertices[5000];		//	on the stack
//sVertex* pVertices = new sVertex[5000];			// On the heap
sVertex* pVertices = NULL;

//float x, y;			// Position
//float r, g, b;	

//	float x, y, z;		vPosition
//	float r, g, b;		vColour

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vColour;\n"
"attribute vec3 vPosition;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    vec3 vertPosition = vPosition;\n"
"    gl_Position = MVP * vec4(vertPosition, 1.0);\n"
"    color = vColour;\n"
"}\n";
static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";


glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Move the camera (A & D for left and right, along the x axis)
	if (key == GLFW_KEY_A)
	{
		cameraEye.x -= 0.1f;		// Move the camera -0.01f units
	}
	if (key == GLFW_KEY_D)
	{
		cameraEye.x += 0.1f;		// Move the camera +0.01f units
	}

	// Move the camera (Q & E for up and down, along the y axis)
	if (key == GLFW_KEY_Q)
	{
		cameraEye.y -= 0.1f;		// Move the camera -0.01f units
	}
	if (key == GLFW_KEY_E)
	{
		cameraEye.y += 0.1f;		// Move the camera +0.01f units
	}

	// Move the camera (W & S for towards and away, along the z axis)
	if (key == GLFW_KEY_W)
	{
		cameraEye.z -= 0.1f;		// Move the camera -0.01f units
	}
	if (key == GLFW_KEY_S)
	{
		cameraEye.z += 0.1f;		// Move the camera +0.01f units
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
	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;

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
	

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
	glCompileShader(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
	glCompileShader(fragment_shader);

	program = glCreateProgram();

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	//	float x, y, z;		vPosition			"attribute vec3 vPosition;\n"
	//	float r, g, b;		vColour				"attribute vec3 vColour;\n"
//	
	//mvp_location = glGetUniformLocation(program, "MVP");
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
									 program);

	// At this point, the model is loaded into the GPU


	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		//       mat4x4 m, p, mvp;
		glm::mat4 m, p, v, mvp;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		//         mat4x4_identity(m);
		m = glm::mat4(1.0f);

		//mat4x4_rotate_Z(m, m, (float) glfwGetTime());
		glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
			glm::radians(0.0f),	// (float)glfwGetTime(),					// Angle 
			glm::vec3(0.0f, 0.0f, 1.0f));

		m = m * rotateZ;

		glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
			(float)glfwGetTime(),					// Angle 
			glm::vec3(0.0f, 1.0f, 0.0f));

		m = m * rotateY;

		glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
			glm::radians(0.0f),	// (float)glfwGetTime(),					// Angle 
			glm::vec3(1.0f, 0.0f, 0.0f));

		m = m * rotateX;

		// Move it to the +ve x
		glm::mat4 matTrans = glm::translate( glm::mat4(1.0f),
			                                 glm::vec3(0.0f, -1.0f, 0.0f));
		m = m * matTrans;

		// Scale
		glm::mat4 scale = glm::scale(glm::mat4(1.0f),
			                         glm::vec3(1.0f, 1.0f, 1.0f));
		m = m * scale;


		//mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		p = glm::perspective(0.6f,		// FOV
			ratio,			// Aspect ratio
			0.1f,			// Near clipping plane
			1000.0f);		// Far clipping plane

		v = glm::mat4(1.0f);

		//glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0);
		//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

		v = glm::lookAt(cameraEye,
			cameraTarget,
			upVector);

		//mat4x4_mul(mvp, p, m);
		mvp = p * v * m;


		glUseProgram(program);


		//glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

		// This will change the fill mode to something 
		//  GL_FILL is solid 
		//  GL_LINE is "wireframe"
		//glPointSize(15.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


//		glDrawArrays(GL_TRIANGLES, 0, 2844);
//		glDrawArrays(GL_TRIANGLES, 0, numberOfVertsOnGPU);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();


	// Delete everything
	delete pTheModelLoader;
	delete pTheVAOManager;

	// Watch out!!
	// sVertex* pVertices = new sVertex[numberOfVertsOnGPU];
	delete [] pVertices;		// If it's an array, also use [] bracket

	exit(EXIT_SUCCESS);
}
