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
	std::map<std::string, cGameObject*> game_objects;
	std::map<std::string, cLight*> lights;
	cVAOManager* theVAOManager;
	static Scene* theScene;
	Scene();
	void loadObjects(std::string filename);
	void loadLights(std::string filename);
	void drawObjects();
	void drawLights();

public:
	~Scene();
	static Scene* getTheScene();
	void addGameObject(cGameObject* obj);
	std::vector<cGameObject*> getGameObjects();
	cGameObject* findGameObject(std::string name);
	void loadScene(std::string filename);
	void saveScene(std::string filename);
	void drawScene();
};
