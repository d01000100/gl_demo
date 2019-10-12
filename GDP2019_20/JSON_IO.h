#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <map>"
#include "Scene.h"

struct meshSettings {
	std::string name;
	std::string filename;
};

std::vector<meshSettings>* readMeshes(std::string f);

std::map<std::string, cGameObject*>* readObjects(std::string f);

std::map<std::string, cLight*>* readLights(std::string f);

void saveScene(Scene* scene, std::string filename);
nlohmann::json serializeObjects(std::vector<cGameObject*> objs);
