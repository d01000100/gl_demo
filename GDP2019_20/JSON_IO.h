#pragma once
#include <string>
#include <vector>
#include <map>
#include "cGameObject.h"
#include "cLight.h"

struct meshSettings {
	std::string name;
	std::string filename;
};

std::vector<meshSettings>* readMeshes(std::string f);

std::map<std::string, cGameObject*>* readObjects(std::string f);

std::map<std::string, cGameObject*>* readObjects(std::string f);

std::map<std::string, cLight*>* readLights(std::string f);
