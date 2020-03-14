#include "GLCommon.h"
#include "Scene.h"
#include "cModelLoader.h"
#include "globalStuff.h"
#include "JSON_IO.h"
#include "Camera.h"
#include "cGameObject.h"

#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/string_cast.hpp>

Scene* Scene::theScene = new Scene();

// does nothing. Need to "implement" it to make it private
Scene::Scene() { } 

Scene* Scene::getTheScene() {
	return theScene;
}

std::vector<aGameItem*> Scene::getItems() {
	std::vector<aGameItem*> vs;
	for (std::map<std::string, aGameItem*>::iterator i = gameItems.begin();
		i != gameItems.end(); i++)
	{
		vs.push_back(i->second);
	}
	return vs;
}

std::vector<cLight*> Scene::getLights() {

	std::vector<cLight*> vs;
	for (std::map<std::string, cLight*>::iterator i = lights.begin();
		i != lights.end(); i++)
	{
		vs.push_back(i->second);
	}
	return vs;
}

std::vector<cGameObject*> Scene::getGameObjects()
{
	vGameObjects res;
	for (auto item : gameItems)
	{
		if (item.second->getType() == "Object")
		{
			res.push_back((cGameObject*)item.second);
		}
	}
	return res;
}

std::map<std::string, cLight*> Scene::getLightsMap() { return lights;  }
std::map<std::string, sCameraSettings*> Scene::getCamerasMap() { return cameras; }

bool Scene::loadMeshes(std::string filename) {
	std::vector<meshSettings>* vMeshes = readMeshes(filename);

	if (!vMeshes) { return false; }

	GLuint programID = ::theShaderManager.getIDFromFriendlyName(::shader_name);

	for (int i = 0; i < vMeshes->size(); i++) {
		meshSettings settings = vMeshes->at(i);

		if (!cModelLoader::LoadPlyModel(settings.filename, settings.name)) {
			return false;
		}

		if (!::theVAOManager->LoadModelIntoVAO(
			settings.name,
			programID))
		{
			return false;
		}
	}

	return true;
}

void Scene::lookAtActiveCamera() {
	if (activeCamera != cameras.end()) {
		sCameraSettings* settings = activeCamera->second;
		Camera* theCamera = Camera::getTheCamera();
		theCamera->setPosition(settings->position);
		theCamera->setTarget(settings->target);
	}
}

bool Scene::loadCameras(std::string filename) {
	std::map<std::string, sCameraSettings*>* loaded_cameras = readCameras(filename);
	if (loaded_cameras == NULL) { return false; }
	else {
		cameras = *loaded_cameras;
	}
	if (cameras.begin() != cameras.end()) {
		activeCamera = cameras.begin();
		lookAtActiveCamera();
	}
	return true;
}

void Scene::changeCamera() {
	activeCamera++;
	if (activeCamera == cameras.end()) {
		activeCamera = cameras.begin();
	}
	lookAtActiveCamera();
}

bool Scene::loadScene(std::string filename) {
	if (!loadMeshes(filename)) { return false; }

	if (!reloadScene(filename)) { return false; }
	return true;
}

// will not reload Meshes
bool Scene::reloadScene(std::string filename) {
	printf("%s\n", filename.c_str());
	std::map<std::string, aGameItem*>* loaded_items = readItems(filename);
	if (loaded_items == NULL) {return false;  }
	else {
		gameItems.clear();
		gameItems = *loaded_items;
		printf("total items: %d\n", gameItems.size());
	}

	if (!loadCameras(filename)) { return false; }
	return true;
}

void Scene::drawItems() {

	std::map<double, cGameObject*> transparentsByDistance;
	Camera* theCamera = Camera::getTheCamera();

	for (std::map<std::string, aGameItem*>::iterator i = gameItems.begin();
		i != gameItems.end(); i++)
	{
		if (i->second->getType() != "Object") {
			i->second->draw();
		}
		// We exclude objects because we need to consider transparency on thise
		else {
			cGameObject* gameObj = (cGameObject*)i->second;
			// if they are opaque, we draw can draw them
			if (gameObj->alpha == 1) {
				gameObj->draw();
			}
			// if they are transparent, we sort them by the distance to the camera
			// from farthest to closest.
			else {
				double distance = glm::distance(theCamera->getPosition(), gameObj->getPos());
				transparentsByDistance[-distance] = gameObj;
			}
		}
	}

	// we draw the transparent objects from farthest to closest
	for (std::map<double, cGameObject*>::iterator itTransparent = transparentsByDistance.begin();
		itTransparent != transparentsByDistance.end(); itTransparent++)
		itTransparent->second->draw();
}

void Scene::drawScene() {
	drawItems();
}

void Scene::saveScene(std::string filename) {

}

void Scene::IntegrationStep(float deltaTime) {
	for (std::map<std::string, aGameItem*>::iterator itItem = gameItems.begin();
		itItem != gameItems.end();) {
		//sMessage m; m.name = "integration step"; m.fValue = deltaTime;
		//currentItem->recieveMessage(m);

		aGameItem* currentItem = itItem->second;
		if (currentItem->getType() == "Object")
		{
			cGameObject* currentObject = (cGameObject*)currentItem;

			if (currentObject->lifeTime != 0.0f &&
				currentObject->lifeTime < 0.0f)
			{
				delete currentObject;
				currentObject = NULL;
				std::map<std::string, aGameItem*>::iterator toDelete = itItem;
				//itItem++;
				itItem = gameItems.erase(toDelete);
				continue;
			}
		}

		itItem++;
	}
}

std::vector<aGameItem*> Scene::getItemsByType(std::string type) {
	std::vector<aGameItem*> vs;
	for (std::map<std::string, aGameItem*>::iterator i = gameItems.begin();
		i != gameItems.end(); i++)
	{
		aGameItem* item = i->second;
		if (item->getType() == type) {
			vs.push_back(i->second);
		}
	}
	return vs;
}

vGameObjects Scene::getObjsWithTag(std::string tag)
{
	vGameObjects res;
	for (auto item : gameItems)
	{
		if (item.second->getType() == "Object")
		{
			cGameObject* obj = (cGameObject*)item.second;
			if (obj->tags.count(tag))
			{
				res.push_back(obj);
			}
		}
	}
	return res;
}

void Scene::storeCurrentCamera() {

	Camera* theCamera = Camera::getTheCamera();

	sCameraSettings* settings = new sCameraSettings();
	settings->name = glm::to_string(theCamera->getPosition()) +
		" -> " + glm::to_string(theCamera->getTarget());
	settings->position = theCamera->getPosition();
	settings->target = theCamera->getTarget();

	cameras[settings->name] = settings;
}

aGameItem* Scene::findItem(std::string name) {
	if (gameItems.find(name) != gameItems.end()) {
		return gameItems[name];
	}
	else {
		return NULL;
	}
}

void Scene::setCamera(std::string name) {
	if (cameras.find(name) != cameras.end()) {
		activeCamera = cameras.find(name);
	}
	lookAtActiveCamera();
}

void Scene::addItem(aGameItem* newItem)
{
	gameItems[newItem->getName()] = newItem;
}

bool Scene::removeItem(std::string itemName)
{
	if (gameItems.find(itemName) != gameItems.end()) {
		delete gameItems[itemName];
		gameItems.erase(itemName);
		return true;
	}
	return false;
}