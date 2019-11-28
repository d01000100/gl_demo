#include "DollyCamera.h"

#include "GLCommon.h"
#include "globalStuff.h"

DollyCamera* DollyCamera::theDollyCamera = new DollyCamera();

DollyCamera* DollyCamera::getTheCamera()
{
	return theDollyCamera;
}

DollyCamera::DollyCamera() {}

glm::mat4 DollyCamera::lookAt() {

	GLuint shaderProgID = ::theShaderManager.getIDFromFriendlyName(::shader_name);

	// Also set the position of my "eye" into the shader
	GLint eyeLocation_UL = glGetUniformLocation(shaderProgID, "eyeLocation");
	glUniform4f(eyeLocation_UL, pos.x, pos.y, pos.z, 1.0f);

	return glm::lookAt(pos, pos + getDirection(), getUp());
}