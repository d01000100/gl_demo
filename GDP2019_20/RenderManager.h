#pragma once
#include <glm/vec3.hpp>
#include "cFBO.h"
#include "GameItemFactory/iGameItem.h"
#include "Scene.h"
#include "util.h"

struct SceneDefs
{
	Scene* pScene;
	int width, height;
	std::string name;
	cFBO* pFBO;
	SceneDefs() :
		pScene(nullptr),
		width(1600),
		height(800),
		pFBO(nullptr) {}
};

struct RenderManager
{
	static std::map<std::string, SceneDefs*> mScenes;
	/*
	 * Draw a certain scene (collection of game items: objects, lights, skybox)
	 * and output the result to a FBO or to the frame buffer if null.
	 */
	static bool deferredDraw(
		glm::vec3 eyePos, glm::vec3 cameraTarget,
		SceneDefs* sceneData,
		const std::map<std::string, glm::vec4> &shaderProps = {}
	);

	/*
	 * Draw a scene stored in the map of scenes by name.
	 */
	static bool deferredDraw(
		glm::vec3 eyePos, glm::vec3 cameraTarget,
		std::string sceneName,
		const std::map<std::string, glm::vec4>& shaderProps = {}
	)
	{
		if (mapContains(mScenes, sceneName))
			return deferredDraw(eyePos, cameraTarget, mScenes[sceneName], shaderProps);
		else
			return false;
	}

	/*
	 * Get the FBO in which `sceneName` renders to.
	 * Can be nullptr.
	 */
	static cFBO* getFBO(std::string sceneName);
};

