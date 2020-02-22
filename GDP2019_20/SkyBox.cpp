#include "SkyBox.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "globalStuff.h"
#include "cModelLoader.h"
#include "Camera.h"

bool SkyBox::init(std::string posX_fileName, std::string negX_fileName,
				std::string posY_fileName, std::string negY_fileName,
				std::string posZ_fileName, std::string negZ_fileName,
				std::string mesh_name, std::string basepath) 
{
	::g_pTextureManager->SetBasePath(basepath);
	std::string errorString;
	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles(
		"skybox",
		posX_fileName, negX_fileName, 
		posY_fileName, negY_fileName, 
		posZ_fileName, negZ_fileName,
		true, errorString)) 
	{
		printf("Error while loading skybox textures: %s\n", errorString.c_str());
		return false;
	}
	else {
		printf("Skybox Textures loaded\n");
	}

	textureID = ::g_pTextureManager->getTextureIDFromName("skybox");

	this->mesh_name = mesh_name;
	defs.back = negZ_fileName;
	defs.front = posZ_fileName;
	defs.left = negX_fileName;
	defs.right = posX_fileName;
	defs.top = posY_fileName;
	defs.bottom = negY_fileName;
	defs.basepath = basepath;

	return true;
}

void SkyBox::draw(glm::vec3 pos)
{
	GLuint shaderProgID = ::theShaderManager.getIDFromFriendlyName(::shader_name);
	glCullFace(GL_FRONT_AND_BACK);
	glDisable(GL_DEPTH_TEST); // Turn OFF depth test
	glDisable(GL_DEPTH); // Do not Write to depth buffer

	glUniform1f(glGetUniformLocation(shaderProgID, "bIsSkyBox"),
				(float)GL_TRUE);

	glActiveTexture(GL_TEXTURE26);				// Texture Unit 26
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);	// Texture now assoc with texture unit 0
	// Tie the texture units to the samplers in the shader
	glUniform1i(glGetUniformLocation(shaderProgID, "skyBox"), 26);	// Texture unit 26

	glm::mat4x4 transformMatrix = glm::mat4(1.0f); 
	transformMatrix = glm::translate(transformMatrix,pos);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgID, "matModel"),
		1, GL_FALSE, glm::value_ptr(transformMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgID, "matModelInverseTranspose"),
		1, GL_FALSE, glm::value_ptr(glm::inverse(glm::transpose(transformMatrix))));

	sModelDrawInfo drawInfo;
	::theVAOManager->FindDrawInfoByModelName(mesh_name, drawInfo);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
	glBindVertexArray(drawInfo.VAO_ID);
	glDrawElements(GL_TRIANGLES,
		drawInfo.numberOfIndices,
		GL_UNSIGNED_INT,
		0);
	glBindVertexArray(0);

	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST); // Turn OFF depth test
	glEnable(GL_DEPTH); // Do not Write to depth buffer

	glUniform1f(glGetUniformLocation(shaderProgID, "bIsSkyBox"),
		(float)GL_FALSE);
}
