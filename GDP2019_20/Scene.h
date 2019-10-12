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
	cGameObject* findGameObject(std::string name);
	bool loadScene(std::string filename);
	void saveScene(std::string filename);
	void drawScene();
};
