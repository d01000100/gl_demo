#include "cGameObject.h"
#include "globalStuff.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/string_cast.hpp>
#include <sstream>
#include <algorithm>

#include "colors.h"

cGameObject::cGameObject()
{
	this->scale = 1.0f;
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
	position = glm::vec3(0);
	rotationXYZ = glm::vec3(0);
	front = glm::vec3(1.0f, 0.0f, 0.0f);

	alpha = 1.0f;
	isLit = true;
	lifeTime = 0.0f;

	diffuseColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	specularColor = glm::vec4(1.0f, 1.0f, 1.0f, 50.0f);

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

std::vector<glm::vec3> cGameObject::getTransformedCollisionPoints()
{
	glm::mat4 transMat = calculateTransformationMatrix();
	std::vector<glm::vec3> res(collision_points);
	for (int p = 0; p < res.size(); p++)
	{
		glm::vec4 temp(res[p], 1);
		temp = transMat * temp;
		res[p] = temp;
	}
	return res;
}

void drawPoint(glm::vec3 point)
{
	::g_pDebugRenderer->addLine(
		point - glm::vec3(2.0, 0.0, 0.0),
		point + glm::vec3(2.0, 0.0, 0.0), Colors::orange, 0.001f);
	::g_pDebugRenderer->addLine(
		point - glm::vec3(0.0, 2.0, 0.0),
		point + glm::vec3(0.0, 2.0, 0.0), Colors::orange, 0.001f);
	::g_pDebugRenderer->addLine(
		point - glm::vec3(0.0, 0.0, 2.0),
		point + glm::vec3(0.0, 0.0, 2.0), Colors::orange, 0.001f);
}

void cGameObject::draw() 
{
	if (isVisible) {
		GLuint programID = ::theShaderManager.getIDFromFriendlyName(::shader_name);

		sModelDrawInfo drawInfo;
		glm::mat4 m = calculateTransformationMatrix();
		if (::theVAOManager->FindDrawInfoByModelName(meshName, drawInfo))
		{

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
				alpha);

			GLint specularColour_UL = glGetUniformLocation(programID, "specularColour");
			glUniform4f(specularColour_UL,
				specularColor.r,
				specularColor.g,
				specularColor.b,
				specularColor.a);	// Specular "power" (how shinny the object is)
									// 1.0 to really big (10000.0f)

			glUniform1f(glGetUniformLocation(programID, "bDoNotLight"),
				(float)!isLit);

			glUniform1f(glGetUniformLocation(programID, "hasTextures"),
				(float)!textures.empty());
			if (!textures.empty())
			{
				glm::vec4 textureWeights = glm::vec4(0.0f);
				for (int t = 0; t < textures.size(); t++)
				{
					GLuint textureId_UL = ::g_pTextureManager->getTextureIDFromName(textures[t].textureName);
					glActiveTexture(GL_TEXTURE0 + t);
					glBindTexture(GL_TEXTURE_2D, textureId_UL);

					// Tie the texture units to the samplers in the shader
					GLint textSamp_UL = glGetUniformLocation(programID, ("textSamp0" + std::to_string(t)).c_str());
					glUniform1i(textSamp_UL, t);

					textureWeights[t] = textures[t].weight;
				}

				textureWeights = glm::normalize(textureWeights);
				glUniform4f(glGetUniformLocation(programID, "tex_0_3_ratio"),
					textureWeights[0],
					textureWeights[1],
					textureWeights[2],
					textureWeights[3]);

			}

			// Super hack for doing holes in the ship
			bool hasHoles = false; //  (friendlyName.find("cruiseship") != std::string::npos);

			// This surely just needs to be done once
			if (hasHoles) {
				GLuint textureId_UL = ::g_pTextureManager->getTextureIDFromName("dots.bmp");
				glActiveTexture(GL_TEXTURE10);
				glBindTexture(GL_TEXTURE_2D, textureId_UL);

				// Tie the texture units to the samplers in the shader
				GLint textSamp_UL = glGetUniformLocation(programID, "dotsSampler");
				glUniform1i(textSamp_UL, 10);
			}

			GLint isHoled_UL = glGetUniformLocation(programID, "isHoled");
			glUniform1i(isHoled_UL, hasHoles);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
			glEnable(GL_DEPTH_TEST);						// Turn ON depth test
			glEnable(GL_DEPTH);								// Write to depth buffer
			// transparency configs
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glBindVertexArray(drawInfo.VAO_ID);
			glDrawElements(GL_TRIANGLES,
				drawInfo.numberOfIndices,
				GL_UNSIGNED_INT,
				0);
			glBindVertexArray(0);

			std::vector<glm::vec3> points = getTransformedCollisionPoints();
			for (int p = 0; p < points.size(); p++)
			{
				drawPoint(points[p]);
			}

		}
	}
}

void cGameObject::recieveMessage(sMessage message) {

	float translationStep = 1.0f;
	float rotationStep = 0.02;
	float scaleStep = 0.05f;

	if (message.name == "translate") {
		position += glm::normalize(message.v3Value) * translationStep;
	}
	else if (message.name == "rotate") {
		rotationXYZ += glm::normalize(message.v3Value) * rotationStep;
	}
	else if (message.name == "scale") {
		if (message.fValue > 0) {
			scale *= 1.05;
		}
		if (message.fValue < 0) {
			scale *= 0.95;
		}
	}
	else if (message.name == "integration step") {
		IntegrationStep(message.fValue);
	}
	else if (message.name == "apply velocity") {
		if (physics) {

			glm::vec3 newVel = physics->velocity + message.v3Value;
			float newSpeed = std::min(glm::length(newVel), 20.0f);
			if (newSpeed < 0.1f) {
				physics->velocity = glm::vec3(0);
			}
			else {
				newVel = glm::normalize(newVel) * newSpeed;
				physics->velocity = newVel;
			}
		}
	}
	else if (message.name == "stop") {
		if (physics) {
			physics->velocity = glm::vec3(0);
		}
	}
}

std::string cGameObject::getType() { return "Object"; }
std::string cGameObject::getInfo() {
	std::stringstream ss;
	ss << getType() << " - " << getName() <<
		" color: " << glm::to_string(diffuseColor) <<
		" alpha: " << alpha;
	return ss.str();
};

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
	if (lifeTime != 0.0f)
		lifeTime -= deltaTime;
}

// this variable is static, so common to all objects.
// When the object is created, the unique ID is set, and 
//	the next unique ID is incremented
//static 
unsigned int cGameObject::next_uniqueID = 1000;	// Starting at 1000, just because

json cGameObject::toJSON() {
	json jObj;

	jObj["mesh"] = meshName;

	jObj["name"] = friendlyName;

	jObj["position"][0] = position.x;
	jObj["position"][1] = position.y;
	jObj["position"][2] = position.z;

	jObj["rotation"][0] = glm::degrees(rotationXYZ.x);
	jObj["rotation"][1] = glm::degrees(rotationXYZ.y);
	jObj["rotation"][2] = glm::degrees(rotationXYZ.z);

	jObj["scale"] = scale;
	jObj["alpha"] = alpha;

	jObj["diffuseColor"][0] = diffuseColor.x * 255.f;
	jObj["diffuseColor"][1] = diffuseColor.y * 255.f;
	jObj["diffuseColor"][2] = diffuseColor.z * 255.f;

	jObj["specularColor"][0] = specularColor.x * 255.f;
	jObj["specularColor"][1] = specularColor.y * 255.f;
	jObj["specularColor"][2] = specularColor.z * 255.f;
	jObj["specularColor"][3] = specularColor.a;

	jObj["isVisible"] = isVisible;
	jObj["isLit"] = isLit;

	jObj["front"][0] = front.x;
	jObj["front"][1] = front.y;
	jObj["front"][2] = front.z;

	if (physics)
	{
		json jPhysics;

		jPhysics["gravity"] = physics->gravity;

		jPhysics["acceleration"][0] = physics->acceleration.x;
		jPhysics["acceleration"][1] = physics->acceleration.y;
		jPhysics["acceleration"][2] = physics->acceleration.z;

		jPhysics["velocity"][0] = physics->velocity.x;
		jPhysics["velocity"][1] = physics->velocity.y;
		jPhysics["velocity"][2] = physics->velocity.z;

		eShapeTypes shape = physics->shape;
		if (shape == SPHERE) {
			jPhysics["shape"] = "sphere";
		}
		else if (shape == MESH) {
			jPhysics["shape"] = "mesh";
		}
		else if (shape == AABB) {
			jPhysics["shape"] = "aabb";
		}
		else if (shape == CAPSULE) {
			jPhysics["shape"] = "capsule";
		}
		else if (shape == PLANE) {
			jPhysics["shape"] = "plane";
		}
		else {
			jPhysics["shape"] = "???";
		}

		jPhysics["radius"] = physics->radius;

		jObj["Physics"] = jPhysics;
	}

	if (!textures.empty()) {
		json jvTextures;
		for (int t = 0; t < textures.size(); t++) {
			jvTextures[t]["textureName"] = textures[t].textureName;
			jvTextures[t]["weight"] = textures[t].weight;
		}
		jObj["textures"] = jvTextures;
	}

	if (!collision_points.empty()) {
		json jvPoints;
		for (int t = 0; t < collision_points.size(); t++) {
			jvPoints[t][0] = collision_points[t].x;
			jvPoints[t][1] = collision_points[t].y;
			jvPoints[t][2] = collision_points[t].z;
		}
		jObj["collision_points"] = jvPoints;
	}

	return jObj;
}