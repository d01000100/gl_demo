#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <map>
#include "Scene.h"

struct meshSettings {
	std::string name;
	std::string filename;
};

struct JSONConfig
{
	static json jConfig, jScenes;
};

std::vector<meshSettings>* readMeshes(std::string f);

bool readTextures(std::string f, Scene* pScene);

std::map<std::string, sCameraSettings*>* readCameras(std::string filename);

std::map<std::string, aGameItem*>* readItems(std::string filename);

void saveScene(Scene* scene, std::string filename);

bool loadScenes(std::string filename);

bool readSkybox(std::string filename, SkyBox* &skybox);

//nlohmann::json serializeObjects(std::vector<cGameObject*> objs);