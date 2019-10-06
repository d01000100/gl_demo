#pragma once
#include <string>
#include <map>
#include <vector>
#include "cGameObject.h"
#include "cVAOManager.h"

class Scene
{
private:
	std::map<std::string, cGameObject*> game_objects;
	static Scene* theScene;
	Scene();
	cVAOManager* theVAOManager;

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
