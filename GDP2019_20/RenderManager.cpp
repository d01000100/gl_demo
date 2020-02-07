#include "RenderManager.h"
#include "globalStuff.h"
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include "SceneEditor.h"

std::map<std::string, SceneDefs*> RenderManager::mScenes;
SceneDefs* RenderManager::sceneOnEdition = nullptr;

bool RenderManager::deferredDraw(
	glm::vec3 eyePos, glm::vec3 cameraTarget, 
	SceneDefs *sceneDefs,
	const std::map<std::string, glm::vec4> &shaderProps)
{
	// TODO: Normalize shader variables to vec4 to be able to set
	// them generically

	cFBO *pFBO = sceneDefs->pFBO;
	Scene* scene = sceneDefs->pScene;
	if (pFBO)
	{
		// 1. Enable the FBO
		glBindFramebuffer(GL_FRAMEBUFFER, pFBO->ID);
		// 2. Clear the FBO
		pFBO->clearBuffers(true, true);
	}
	else
	{
		// 1. Disable de FBO
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// 2. Clear the ACTUAL frame buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	// 3. Move the camera
	::viewTransform = glm::lookAt(eyePos, cameraTarget, glm::vec3(0, 1, 0));
	glUniform4f(
		glGetUniformLocation(g_programID, "eyeLocation") ,
		eyePos.x, eyePos.y, eyePos.z, 1.0f
	);
	glUniformMatrix4fv(
		glGetUniformLocation(g_programID, "matView"), 
		1, GL_FALSE, glm::value_ptr(::viewTransform)
	);
	// 5. Draw scene
	scene->drawScene();

	if (sceneOnEdition == sceneDefs)
	{
		SceneEditor::getTheEditor()->addDebugMarkers();
		SceneEditor::getTheEditor()->getDebugRenderer()->RenderDebugObjects(::viewTransform, ::projTransform, 0.01f);
	}
	
	return true;
}

cFBO* RenderManager::getFBO(std::string sceneName)
{
	if (!mapContains(mScenes, sceneName)) { return nullptr; }
	return mScenes[sceneName]->pFBO;
}
