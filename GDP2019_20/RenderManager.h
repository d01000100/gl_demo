#pragma once
#include <glm/vec3.hpp>
#include "cFBO.h"
#include "GameItemFactory/iGameItem.h"
#include "Scene.h"

struct RenderManager
{
	/*
	 * Draw a certain scene (collection of game items: objects, lights, skybox)
	 * and output the result to a FBO or to the frame buffer if null.
	 */
	static bool deferredDraw(
		glm::vec3 eyePos, glm::vec3 cameraTarget,
		cFBO* pFBO, 
		Scene* scene,
		const std::map<std::string, glm::vec4> &shaderProps = {}
	);
};

