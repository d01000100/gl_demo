#include "RenderManager.h"
#include "globalStuff.h"
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include "SceneEditor.h"
#include <iostream>

std::map<std::string, SceneDefs*> RenderManager::mScenes;
SceneDefs* RenderManager::sceneOnEdition = nullptr;

void RenderManager::setUpCamera(Camera* camera)
{
	setUpCamera(camera->getPosition(), camera->getTarget());
}

void RenderManager::setUpCamera(glm::vec3 eyePos, glm::vec3 cameraTarget)
{
	::viewTransform = glm::lookAt(eyePos, cameraTarget, glm::vec3(0, 1, 0));
	glUniform4f(
		glGetUniformLocation(g_programID, "eyeLocation"),
		eyePos.x, eyePos.y, eyePos.z, 1.0f
	);
	glUniformMatrix4fv(
		glGetUniformLocation(g_programID, "matView"),
		1, GL_FALSE, glm::value_ptr(::viewTransform)
	);
}

void RenderManager::setUpProjection(SceneDefs* sceneData)
{
	float ratio = (float)sceneData->width / (float)sceneData->height;
	::projTransform = glm::perspective(
		0.6f,		// FOV
		ratio,			// Aspect ratio
		0.1f,			// Near clipping plane
		10000.0f);		// Far clipping plane

	glUniformMatrix4fv(
		glGetUniformLocation(g_programID, "matProj")
		, 1, GL_FALSE,
		glm::value_ptr(::projTransform));
	glViewport(0, 0, sceneData->width, sceneData->height);

	glUniform1f(
		glGetUniformLocation(::g_programID, "screenWidth"),
		sceneData->width
	);
	glUniform1f(
		glGetUniformLocation(::g_programID, "screenHeight"),
		sceneData->height
	);
}

void RenderManager::drawEditor(SceneDefs* sceneData)
{
	if (sceneOnEdition == sceneData)
	{
		SceneEditor::getTheEditor()->addDebugMarkers();
		SceneEditor::getTheEditor()->getDebugRenderer()->RenderDebugObjects(::viewTransform, ::projTransform, 0.01f);
	}
}

bool RenderManager::deferredDraw(
	glm::vec3 eyePos, glm::vec3 cameraTarget, 
	std::string sceneName,
	const std::map<std::string, glm::vec4> &shaderProps)
{
	if (!mapContains(mScenes, sceneName)) { return false; }
	auto sceneData = mScenes[sceneName];
	cFBO *pFBO = sceneData->pFBO;
	Scene* scene = sceneData->pScene;
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
	// Projection with custom height and width
	setUpProjection(sceneData);
	// 3. Move the camera
	setUpCamera(eyePos, cameraTarget);
	if (scene->pSkyBox)
		scene->pSkyBox->draw(eyePos);
	// 5. Draw scene
	scene->drawScene();

	drawEditor(sceneData);
	
	return true;
}

bool RenderManager::deferredDraw(
	Camera* camera, 
	std::string sceneName,
	const std::map<std::string, glm::vec4>& shaderProps)
{
	return deferredDraw(
		camera->getPosition(),
		camera->getTarget(),
		sceneName,
		shaderProps);
}

cFBO* RenderManager::getFBO(std::string sceneName)
{
	if (!mapContains(mScenes, sceneName)) { return nullptr; }
	return mScenes[sceneName]->pFBO;
}

bool RenderManager::renderStencilPortal(
	Camera* camera, 
	std::string outerScene, 
	std::string portalScene,
	std::string innerScene)
{
	if (!mapContains(mScenes, outerScene) ||
		!mapContains(mScenes, portalScene) || 
		!mapContains(mScenes, innerScene))
	{
		std::cout << "Some scene is missing in the stencil pass!!\n";
		return false;
	}
	auto outerData = mScenes[outerScene];
	auto portalData = mScenes[portalScene];
	auto innerData = mScenes[innerScene];

	/*
	 * The deferred-ness of this scene is defined only by the `outerScene`
	 */
	if (outerData->pFBO)
	{
		// 1. Enable the FBO
		glBindFramebuffer(GL_FRAMEBUFFER, outerData->pFBO->ID);
		// 2. Clear the FBO
		outerData->pFBO->clearBuffers(true, true);
	} else
	{
		// 1. Disable de FBO
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// 2. Clear the ACTUAL frame buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	// Same with the height and width of the screen
	setUpProjection(outerData);
	// 3. Move the camera
	setUpCamera(camera->getPosition(), camera->getTarget());

	// Settings for the outer scene
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glDisable(GL_STENCIL_TEST);	// Disable stencil test
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	if (outerData->pScene->pSkyBox)
		outerData->pScene->pSkyBox->draw(camera->getPosition());
	outerData->pScene->drawScene();
	drawEditor(outerData);

	/*
	 * Write 1's on the stencil where the portals are
	 */
	glClearStencil(0);
	// Clear stencil 
	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP,// Stencil fails KEEP the original value 
		GL_KEEP,		// Depth fails KEEP the original value
		GL_REPLACE);	// Stencil AND depth PASSES, REPLACE with 1's

	glStencilFunc(
		GL_ALWAYS, // If is succeed, ALWAYS do this
		1,		   // Replace with 1's
		0xFF);	   // Mask of 1111,1111 (no mask)

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	// Depth TEST is still active, 
	// but I don't write to the buffer; 
	glDepthMask(GL_FALSE);

	portalData->pScene->drawScene();
	drawEditor(portalData);

	/*
	 * Draw the scene inside of the stencil
	 */
	// Turn on writing to the depth buffer
	glDepthMask(GL_TRUE);
	// Clear the depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	// Change the stencil operation to not write in it
	glStencilOp(GL_KEEP,		// Stencil fails KEEP the original value (47)
		GL_KEEP,		// (stencil passes) Depth fails KEEP the original value
		GL_KEEP);		// Stencil AND depth PASSES, Keep 1's
	// Change the stencil test to draw only of equal to 1
	glStencilFunc(GL_EQUAL,		// Test if equal
		1,			//
		0xFF);

	if (innerData->pScene->pSkyBox)
		innerData->pScene->pSkyBox->draw(camera->getPosition());
	innerData->pScene->drawScene();
	drawEditor(innerData);

	// Reset everything to draw a scene normally
	glDisable(GL_STENCIL_TEST);
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	return true;
}
