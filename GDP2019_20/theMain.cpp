#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>

struct sVertex
{
	float x, y, z;
	float r, g, b;
};



//sVertex vertices[2844] =
//{
//	{ -0.0248608, 0.122913, 0.0245429,  1.0f, 1.0f, 1.0f },
//
//};

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


	//float scaleFactor = 4.0f;
	//for (unsigned int index = 0; index != 2844; index++)
	//{
	//	vertices[index].x *= scaleFactor;
	//	vertices[index].y *= scaleFactor;
	//}

	// NOTE: OpenGL error checks have been omitted for brevity
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
	
	mvp_location = glGetUniformLocation(program, "MVP");
	vpos_location = glGetAttribLocation(program, "vPosition");
	vcol_location = glGetAttribLocation(program, "vColour");

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
		sizeof(vertices[0]), (void*)0);

	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
		sizeof(vertices[0]), (void*)(sizeof(float) * 3));







	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		//       mat4x4 m, p, mvp;
		glm::mat4 m, p, v, mvp;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

//		glViewport(0, 0, 640, 480);
		glClear(GL_COLOR_BUFFER_BIT);

		//         mat4x4_identity(m);
		m = glm::mat4(1.0f);

		//mat4x4_rotate_Z(m, m, (float) glfwGetTime());
		glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
			glm::radians(0.0f),	// (float)glfwGetTime(),					// Angle 
			glm::vec3(0.0f, 0.0f, 1.0f));

		m = m * rotateZ;

		// Move it to the +ve x
		glm::mat4 matTrans = glm::translate( glm::mat4(1.0f),
			                                 glm::vec3(0.0f, -1.0f, 0.0f));
		m = m * matTrans;

		// Scale
		glm::mat4 scale = glm::scale(glm::mat4(1.0f),
			                         glm::vec3(6.0f, 6.0f, 6.0f));
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


		glDrawArrays(GL_TRIANGLES, 0, 2844);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
