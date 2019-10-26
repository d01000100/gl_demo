#pragma once
#include <string>
#include <map>
#include <vector>
#include "cLight.h"
#include "GameItemFactory/iGameItem.h"
#include "cMesh.h"

class Scene
{
private:
	std::map<std::string, cMesh*> meshes;
	std::map<std::string, cLight*> lights;
	std::map<std::string, iGameItem*> gameItems;
	static Scene* theScene;
	Scene();
	bool loadMeshes(std::string filename);
	bool loadLights(std::string filename);
	void drawLights();
	void drawItems();

public:
	static Scene* getTheScene();
	std::vector<iGameItem*> getItems();
	std::vector<cLight*> getLights();
	std::map<std::string, cLight*> getLightsMap();
	std::map<std::string, cMesh*> getMeshesMap();
	bool loadScene(std::string filename);
	bool reloadScene(std::string filename);
	void saveScene(std::string filename);
	void drawScene();
	void IntegrationStep(float deltaTime);
	std::vector<iGameItem*> getItemsPerType(std::string type);
};
