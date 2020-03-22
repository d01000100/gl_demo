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
#include "RenderManager.h"

cGameObject::cGameObject() :
	deferredTexture("")
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

void cGameObject::setPos(glm::vec3 pos) { position = pos; }

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
	//glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
	//	rotationXYZ.z,	// Angle 
	//	glm::vec3(0.0f, 0.0f, 1.0f));
	//m = m * rotateZ;
	//glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
	//	rotationXYZ.y,
	//	glm::vec3(0.0f, 1.0f, 0.0f));
	//m = m * rotateY;
	//glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
	//	rotationXYZ.x,
	//	glm::vec3(1.0f, 0.0f, 0.0f));
	//m = m * rotateX;

	m *= glm::mat4(getOrientationQ());

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

			glUniform1i(glGetUniformLocation(g_programID, "isDeferredTexture"),
				deferredTexture != "");
			glUniform1i(glGetUniformLocation(g_programID, "usesScreenUVs"),
				friendlyName == "final_pass_quad");
			glUniform1i(glGetUniformLocation(g_programID, "isReflection"),
				friendlyName == "reflection");
			glUniform1i(glGetUniformLocation(g_programID, "isRefraction"),
				friendlyName == "refraction");
			if(deferredTexture != "")
			{
				if (RenderManager::getFBO(deferredTexture))
				{
					// Tie the texture to the texture unitd
					glActiveTexture(GL_TEXTURE0 + 40); // Texture Unit 40!!
					glBindTexture(GL_TEXTURE_2D, RenderManager::getFBO(deferredTexture)->colourTexture_0_ID);	// Texture now assoc with texture unit 40
					GLint textSamp00_UL = glGetUniformLocation(g_programID, "secondPassColourTexture");
					glUniform1i(textSamp00_UL, 40);	// Texture unit 40
				}
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

			GLint isSkinnedMesh_UniLoc = glad_glGetUniformLocation(programID, "isSkinnedMesh");
			if (animManager)
			{
				glUniform1f(isSkinnedMesh_UniLoc, (float)GL_TRUE);

				// Taken from "Skinned Mesh 2 - todo.docx"
				std::vector< glm::mat4x4 > vecFinalTransformation;
				std::vector< glm::mat4x4 > vecOffsets;
				std::vector< glm::mat4x4 > vecObjectBoneTransformation;

				// This loads the bone transforms from the animation model
				animManager->update(
					vecFinalTransformation,
					vecObjectBoneTransformation,
					vecOffsets);

				// Copy all 100 bones to the shader
				GLint matBonesArray_UniLoc = glGetUniformLocation(programID, "matBonesArray");
				// The "100" is to pass 100 values, starting at the pointer location of matBones[0];
				//glUniformMatrix4fv(matBonesArray_UniLoc, 100, GL_FALSE, glm::value_ptr(matBones[0]));

				GLint numBonesUsed = (GLint)vecFinalTransformation.size();

				glUniformMatrix4fv(matBonesArray_UniLoc, numBonesUsed,
					GL_FALSE,
					glm::value_ptr(vecFinalTransformation[0]));
			} else
			{
				glUniform1f(isSkinnedMesh_UniLoc, (float)false);
			}

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
			glEnable(GL_DEPTH_TEST);						// Turn ON depth test
			//glEnable(GL_DEPTH);								// Write to depth buffer
			// transparency configs
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glBindVertexArray(drawInfo.VAO_ID);
			glDrawElements(GL_TRIANGLES,
				drawInfo.numberOfIndices,
				GL_UNSIGNED_INT,
				0);
			glBindVertexArray(0);

			if (::isDebug) {
				std::vector<glm::vec3> points = getTransformedCollisionPoints();
				for (int p = 0; p < points.size(); p++)
				{
					drawPoint(points[p]);
				}
			}

		}
	}
}

void cGameObject::recieveMessage(sMessage message) {

	float translationStep = 1.0f;
	float rotationStep = 1;
	float scaleStep = 0.05f;

	//printf("recieved %s message with v3Value: %s\n",
	//	message.name.c_str(),
	//	glm::to_string(message.v3Value).c_str());

	if (message.name == "translate") {
		position += glm::normalize(message.v3Value) * translationStep;
	}
	else if (message.name == "rotate") {
		addOrientation(glm::normalize(message.v3Value) * rotationStep);
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
		" pos: " << glm::to_string(getPos()) <<
		" orientation: " << glm::to_string(getOrientationEuler()) <<
		" color: " << glm::to_string(diffuseColor) <<
		" alpha: " << alpha;
	return ss.str();
};

void cGameObject::IntegrationStep(float deltaTime) {
	if (physics)
	{
		if (physics->gravity) {
			physics->acceleration += glm::vec3(0.0f, -1.0f, 0.0f) * deltaTime;
		}

		printf("deltaTime: %f\n", deltaTime);
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

	jObj["position"][0] = position.x;
	jObj["position"][1] = position.y;
	jObj["position"][2] = position.z;

	glm::vec3 rotationXYZ = getOrientationEuler();
	jObj["rotation"][0] = rotationXYZ.x;
	jObj["rotation"][1] = rotationXYZ.y;
	jObj["rotation"][2] = rotationXYZ.z;

	jObj["scale"] = scale;
	jObj["isVisible"] = isVisible;

	return jObj;
}