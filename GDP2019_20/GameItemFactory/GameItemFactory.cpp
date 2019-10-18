#include "GameItemFactory.h"
#include "iGameItem.h"
#include "../cGameObject.h"

using namespace nlohmann;

iGameItem* createGameItem(std::string type, json info) {
	if (type == "Object") {
		cGameObject* gameObj = new cGameObject();

		if (info.find("mesh") == info.end()) {
			printf("Object without mesh!!\n");
			return NULL;
		}
		else {
			gameObj->meshName = info["mesh"].get<std::string>();
		}

		if (info.find("name") != info.end()) {
			gameObj->friendlyName = info["name"].get<std::string>();
		}
		else {
			gameObj->friendlyName = std::to_string(gameObj->getUniqueID());
		}

		if (info.find("position") != info.end()) {
			float x = info["position"][0].get<float>();
			float y = info["position"][1].get<float>();
			float z = info["position"][2].get<float>();
			gameObj->position = glm::vec3(x, y, z);
		}
		else {
			gameObj->position = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		if (info.find("rotation") != info.end()) {
			float x = glm::radians(info["rotation"][0].get<float>());
			float y = glm::radians(info["rotation"][1].get<float>());
			float z = glm::radians(info["rotation"][2].get<float>());
			gameObj->rotationXYZ = glm::vec3(x, y, z);
		}
		else {
			gameObj->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		if (info.find("scale") != info.end()) {
			gameObj->scale = info["scale"].get<float>();
		}
		else {
			gameObj->scale = 1.0f;
		}

		if (info.find("diffuseColor") != info.end()) {
			float x = info["diffuseColor"][0].get<float>();
			float y = info["diffuseColor"][1].get<float>();
			float z = info["diffuseColor"][2].get<float>();
			gameObj->diffuseColor = glm::vec4(x, y, z, 1.0f);
		}
		else {
			gameObj->diffuseColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		}

		if (info.find("specularColor") != info.end()) {
			float x = info["specularColor"][0].get<float>();
			float y = info["specularColor"][1].get<float>();
			float z = info["specularColor"][2].get<float>();
			float w = info["specularColor"][3].get<float>();
			gameObj->specularColor = glm::vec4(x, y, z, w);
		}
		else {
			gameObj->specularColor = glm::vec4(1.0f, 1.0f, 1.0f, 50.0f);
		}

		if (info.find("isVisible") != info.end()) {
			gameObj->isVisible = info["isVisible"].get<bool>();
		}
		else {
			gameObj->isVisible = true;
		}

		if (info.find("front") != info.end()) {
			float x = info["front"][0].get<float>();
			float y = info["front"][1].get<float>();
			float z = info["front"][2].get<float>();
			gameObj->front = glm::vec3(x, y, z);
		}
		else {
			gameObj->front = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
		}

		return gameObj;
	}
	else {
		printf("Unrecognized GameItem %s\n", type.c_str());
		return NULL;
	}
}