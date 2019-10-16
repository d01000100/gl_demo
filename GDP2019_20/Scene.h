#pragma once
#include <string>
#include <map>
#include <vector>
#include "cGameObject.h"
#include "cVAOManager.h"
#include "cLight.h"

class Scene
{
private:
	std::map<std::string, cMesh*> meshes;
	std::map<std::string, cGameObject*> game_objects;
	std::map<std::string, cLight*> lights;
	cVAOManager* theVAOManager;
	static Scene* theScene;
	Scene();
	bool loadObjects(std::string filename);
	bool loadMeshes(std::string filename);
	bool loadLights(std::string filename);
	void drawObjects();
	void drawLights();

public:
	~Scene();
	static Scene* getTheScene();
	void addGameObject(cGameObject* obj);
	std::vector<cGameObject*> getGameObjects();
	std::vector<cLight*> getLights();
	std::map<std::string, cLight*> getLightsMap();
	std::map<std::string, cMesh*> getMeshesMap();
	cGameObject* findGameObject(std::string name);
	cLight* findLight(std::string name);
	bool loadScene(std::string filename);
	bool reloadScene(std::string filename);
	void saveScene(std::string filename);
	void drawScene();
};
