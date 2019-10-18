#include "cGameObject.h"
#include "globalStuff.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

cGameObject::cGameObject()
{
	this->scale = 0.0f;
	this->isVisible = true;

	this->isWireframe = false;

	physics = NULL;

	// Set the unique ID
	// Take the value of the static int, 
	//  set this to the instance variable
	this->m_uniqueID = cGameObject::next_uniqueID;
	// Then increment the static variable
	cGameObject::next_uniqueID++;

	this->m_pDebugRenderer = NULL;

	return;
}


unsigned int cGameObject::getUniqueID(void)
{
	return this->m_uniqueID;
}

void cGameObject::setDebugRenderer(iDebugRenderer* pDebugRenderer)
{
	this->m_pDebugRenderer = pDebugRenderer;
	return;
}

std::string cGameObject::getName() {
	return friendlyName;
}

glm::vec3 cGameObject::getPos() { return position; }

glm::mat4 cGameObject::calculateTransformationMatrix() {

	glm::mat4 m = glm::mat4(1.0f);

	// ******* TRANSLATION TRANSFORM *********
	glm::mat4 matTrans
		= glm::translate(glm::mat4(1.0f),
			glm::vec3(position.x,
				position.y,
				position.z));
	m = m * matTrans;

	//// ******* ROTATION TRANSFORM *********
	glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
		rotationXYZ.z,	// Angle 
		glm::vec3(0.0f, 0.0f, 1.0f));
	m = m * rotateZ;

	glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
		rotationXYZ.y,
		glm::vec3(0.0f, 1.0f, 0.0f));
	m = m * rotateY;

	glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
		rotationXYZ.x,
		glm::vec3(1.0f, 0.0f, 0.0f));
	m = m * rotateX;

	// ******* SCALE TRANSFORM *********
	glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
		glm::vec3(scale,
			scale,
			scale));
	m = m * matScale;

	matWorld = m;
	return m;
}

void cGameObject::draw() 
{
	GLuint programID = ::theShaderManager.getIDFromFriendlyName(::shader_name);

	sModelDrawInfo drawInfo;
	if (::theVAOManager->FindDrawInfoByModelName(meshName, drawInfo))
	{
		glm::mat4 m = calculateTransformationMatrix();

		GLint matModel_UL = glGetUniformLocation(programID, "matModel");
		glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(m));

		// Calcualte the inverse transpose of the model matrix and pass that...
		// Stripping away scaling and translation, leaving only rotation
		// Because the normal is only a direction, really
		GLint matModelIT_UL = glGetUniformLocation(programID, "matModelInverseTranspose");
		glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(m));
		glUniformMatrix4fv(matModelIT_UL, 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));

		GLint diffuseColour_UL = glGetUniformLocation(programID, "diffuseColour");
		glUniform4f(diffuseColour_UL,
			diffuseColor.r,
			diffuseColor.g,
			diffuseColor.b,
			diffuseColor.a);

		GLint specularColour_UL = glGetUniformLocation(programID, "specularColour");
		glUniform4f(specularColour_UL,
			specularColor.r,
			specularColor.g,
			specularColor.b,
			1000.0f);	// Specular "power" (how shinny the object is)
						// 1.0 to really big (10000.0f)

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
		glEnable(GL_DEPTH_TEST);						// Turn ON depth test
		glEnable(GL_DEPTH);								// Write to depth buffer

		glBindVertexArray(drawInfo.VAO_ID);
		glDrawElements(GL_TRIANGLES,
			drawInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0);
		glBindVertexArray(0);
	}
}

void cGameObject::recieveMessage(sMessage message) {

	float translationStep = 0.1f;
	float rotationStep = 0.02;

	if (message.name == "translate") {
		position += glm::normalize(message.v3Value) * translationStep;
	}
	else if (message.name == "rotate") {
		rotationXYZ += glm::normalize(message.v3Value) * rotationStep;
	} 
	else if (message.name == "integration step") {
		IntegrationStep(message.fValue);
	}
	else {
		printf("Unrecognized Message to GameObject: %s\n", message.name.c_str());
	}
}

std::string cGameObject::getType() { return "Object"; }

void cGameObject::IntegrationStep(float deltaTime) {
	if (physics)
	{
		if (physics->gravity) {
			physics->acceleration += glm::vec3(0.0f, -1.0f, 0.0f);
		}

		// Forward Explicit Euler Inetegration
		//NewVelocty += Velocity + ( Ax * DeltaTime )
		physics->velocity += physics->acceleration * deltaTime;
		position += physics->velocity * deltaTime;
	}
}

// this variable is static, so common to all objects.
// When the object is created, the unique ID is set, and 
//	the next unique ID is incremented
//static 
unsigned int cGameObject::next_uniqueID = 1000;	// Starting at 1000, just because
