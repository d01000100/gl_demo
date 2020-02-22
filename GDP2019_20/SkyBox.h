#pragma once

#include <string>
#include "GLCommon.h"
#include "cVAOManager.h"

struct SkyBoxDef
{
	std::string front, back, top, bottom, left, right, basepath;
};

struct SkyBox
{
	GLuint textureID = 0;
	std::string mesh_name = "sphere_model";
	SkyBoxDef defs;
	void draw(glm::vec3 pos);
	bool init(std::string posX_fileName, std::string negX_fileName,
	          std::string posY_fileName, std::string negY_fileName,
	          std::string posZ_fileName, std::string negZ_fileName,
	          std::string mesh_name, std::string basepath);
};