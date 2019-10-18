#include "GameItemFactory.h"
#include "iGameItem.h"
#include "../cGameObject.h"

using namespace nlohmann;

sPhysicsObject* buildPhysicsObject(json jPhysics) {

	sPhysicsObject* physics = new sPhysicsObject();

	if (jPhysics.find("gravity") != jPhysics.end()) {
		physics->gravity = jPhysics["gravity"].get<bool>();
	}
	else {
		physics->gravity = false;
	}

	if (jPhysics.find("speed") != jPhysics.end()) {
		physics->speed = jPhysics["speed"].get<float>();
	}
	else {
		physics->speed = 0.0f;
	}

	if (jPhysics.find("acceleration") != jPhysics.end()) {
		float x = jPhysics["acceleration"][0].get<float>();
		float y = jPhysics["acceleration"][1].get<float>();
		float z = jPhysics["acceleration"][2].get<float>();
		physics->acceleration = glm::vec3(x, y, z);
	}
	else {
		physics->acceleration = glm::vec3(0.0f);
	}

	if (jPhysics.find("velocity") != jPhysics.end()) {
		float x = jPhysics["velocity"][0].get<float>();
		float y = jPhysics["velocity"][1].get<float>();
		float z = jPhysics["velocity"][2].get<float>();
		physics->velocity = glm::vec3(x, y, z);
	}
	else {
		physics->velocity = glm::vec3(0.0f);
	}

	if (jPhysics.find("shape") != jPhysics.end()) {
		std::string shape = jPhysics["shape"].get<std::string>();
		if (shape == "sphere") {
			physics->shape = SPHERE;
		}
		else if (shape == "mesh") {
			physics->shape = MESH;
		}
		else if (shape == "aabb") {
			physics->shape = AABB;
		}
		else if (shape == "capsule") {
			physics->shape = CAPSULE;
		}
		else if (shape == "plane") {
			physics->shape = PLANE;
		}
		else {
			physics->shape = UNKNOWN;
		}
	}
	else {
		physics->shape = UNKNOWN;
	}

	if (jPhysics.find("radius") != jPhysics.end()) {
		physics->radius = jPhysics["radius"].get<float>();
	}

	return physics;
}

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

		if (info.find("Physics") != info.end()) {
			gameObj->physics = buildPhysicsObject(info["Physics"]);
		}

		return gameObj;
	}
	else {
		printf("Unrecognized GameItem %s\n", type.c_str());
		return NULL;
	}
}