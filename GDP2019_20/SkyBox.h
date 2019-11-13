#pragma once

#include <string>
#include "GLCommon.h"
#include "cVAOManager.h"

class SkyBox
{
private:
	GLuint textureID;
	std::string mesh_name;
public:
	void draw();
	bool init(std::string posX_fileName, std::string negX_fileName,
		std::string posY_fileName, std::string negY_fileName,
		std::string posZ_fileName, std::string negZ_fileName,
		std::string mesh_name);
};