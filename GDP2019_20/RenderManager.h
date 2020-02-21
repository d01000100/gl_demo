#pragma once
#include <glm/vec3.hpp>
#include "cFBO.h"
#include "GameItemFactory/iGameItem.h"
#include "Scene.h"
#include "util.h"
#include "Camera.h"

struct SceneDefs
{
	Scene* pScene;
	int width, height;
	std::string name, pathfile;
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
	static SceneDefs* sceneOnEdition;
	static void setUpCamera(Camera* camera);
	static void setUpCamera(glm::vec3, glm::vec3);
	static void setUpProjection(SceneDefs* sceneData);
	static void drawEditor(SceneDefs* sceneData);
	/*
	 * Draw a scene stored in the map of scenes by name.
	 * and output the result to a FBO or to the frame buffer if null.
	 * a scene: (collection of game items: objects, lights, skybox)
	 */
	static bool deferredDraw(
		glm::vec3 eyePos, glm::vec3 cameraTarget,
		std::string sceneName,
		const std::map<std::string, glm::vec4>& shaderProps = {}
	);
	static bool deferredDraw(
		Camera *camera,
		std::string sceneName,
		const std::map<std::string, glm::vec4>& shaderProps = {}
	);

	/*
	 * Get the FBO in which `sceneName` renders to.
	 * Can be nullptr.
	 */
	static cFBO* getFBO(std::string sceneName);

	static bool renderStencilPortal(
		Camera* camera,
		std::string outerScene,
		std::string portalScene,
		std::string innerScene);
};

