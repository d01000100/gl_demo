#pragma once
#include <string>
#include <map>
#include <vector>
#include "cLight.h"
#include "GameItemFactory/iGameItem.h"
#include "cMesh.h"
#include "CameraSettings.h"

class Scene
{
private:
	std::map<std::string, cMesh*> meshes;
	std::map<std::string, cLight*> lights;
	std::map<std::string, aGameItem*> gameItems;
	std::map<std::string, sCameraSettings*> cameras;
	std::map<std::string, sCameraSettings*>::iterator activeCamera;
	static Scene* theScene;
	Scene();
	bool loadMeshes(std::string filename);
	bool loadCameras(std::string filename);
	bool loadLights(std::string filename);
	void drawLights();
	void drawItems();
	void lookAtActiveCamera();

public:
	static Scene* getTheScene();
	std::vector<aGameItem*> getItems();
	std::vector<cLight*> getLights();
	std::map<std::string, cLight*> getLightsMap();
	std::map<std::string, cMesh*> getMeshesMap();
	std::map<std::string, sCameraSettings*> getCamerasMap();
	void changeCamera();
	bool loadScene(std::string filename);
	bool reloadScene(std::string filename);
	void saveScene(std::string filename);
	void drawScene();
	void IntegrationStep(float deltaTime);
	std::vector<aGameItem*> getItemsByType(std::string type);
	void storeCurrentCamera();
	void setCamera(std::string);
	aGameItem* findItem(std::string);
	void addItem(aGameItem* newItem);
};
