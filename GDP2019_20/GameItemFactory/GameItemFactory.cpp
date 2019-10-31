#include "../globalStuff.h"

#include "GameItemFactory.h"
#include "iGameItem.h"
#include "../cGameObject.h"
#include "../cLight.h"
#include "../cGameAudio.h"

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
			gameObj->friendlyName = gameObj->meshName + "_" + std::to_string(gameObj->getUniqueID());
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
			float x = info["diffuseColor"][0].get<float>() / 255.0f;
			float y = info["diffuseColor"][1].get<float>() / 255.0f;
			float z = info["diffuseColor"][2].get<float>() / 255.0f;
			gameObj->diffuseColor = glm::vec4(x, y, z, 1.0f);
		}
		else {
			gameObj->diffuseColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		}

		if (info.find("specularColor") != info.end()) {
			float x = info["specularColor"][0].get<float>() / 255.0f;
			float y = info["specularColor"][1].get<float>() / 255.0f;
			float z = info["specularColor"][2].get<float>() / 255.0f;
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
	else if (type == "Light") {
		cLight* light = new cLight();

		if (info.find("name") == info.end()) {
			light->name = "light_" + std::to_string(light->getUniqueId());
		}
		else {
			light->name = info["name"].get<std::string>();
		}

		if (info.find("position") != info.end()) {
			float x = info["position"][0].get<float>();
			float y = info["position"][1].get<float>();
			float z = info["position"][2].get<float>();
			light->pos = glm::vec3(x, y, z);
		}
		else {
			light->pos = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		if (info.find("diffuseColor") != info.end()) {
			float x = info["diffuseColor"][0].get<float>() / 255.0f;
			float y = info["diffuseColor"][1].get<float>() / 255.0f;
			float z = info["diffuseColor"][2].get<float>() / 255.0f;
			light->diffuseColor = glm::vec3(x, y, z);
		}
		else {
			light->diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
		}

		if (info.find("specularColor") != info.end()) {
			float x = info["specularColor"][0].get<float>() / 255.0f;
			float y = info["specularColor"][1].get<float>() / 255.0f;
			float z = info["specularColor"][2].get<float>() / 255.0f;
			float w = info["specularColor"][3].get<float>();
			light->specularColor = glm::vec4(x, y, z, w);
		}
		else {
			light->specularColor = glm::vec4(1.0f, 1.0f, 1.0f, 50.0f);
		}

		if (info.find("linearAtten") != info.end()) {
			light->linearAtten = info["linearAtten"].get<float>();
		}
		else {
			light->linearAtten = 0.05f;
		}

		if (info.find("quadAtten") != info.end()) {
			light->quadAtten = info["quadAtten"].get<float>();
		}
		else {
			light->quadAtten = 0.05f;
		}

		if (info.find("cutOffDist") != info.end()) {
			light->cutOffDist = info["cutOffDist"].get<float>();
		}
		else {
			light->cutOffDist = 50.0f;
		}

		if (info.find("type") != info.end()) {
			std::string type = info["type"].get<std::string>();
			if (type == "point") {
				light->type = POINT;
			}
			if (type == "spotlight") {
				light->type = SPOT;
			}
			if (type == "directional") {
				light->type = DIRECTIONAL;
			}
		}
		else {
			light->type = POINT;
		}

		if (info.find("isOn") != info.end()) {
			light->isOn = info["isOn"].get<bool>();
		}
		else {
			light->isOn = true;
		}

		if (info.find("direction") != info.end()) {
			float x = info["direction"][0].get<float>();
			float y = info["direction"][1].get<float>();
			float z = info["direction"][2].get<float>();
			light->direction = glm::vec3(x,y,z);
		}

		if (info.find("innerAngle") != info.end()) {
			light->innerAngle = info["innerAngle"].get<float>();
		}

		if (info.find("outerAngle") != info.end()) {
			light->outerAngle = info["outerAngle"].get<float>();
		}

		return light;
	}
	else if (type == "sound") {

		cGameAudio* gameAudio = new cGameAudio();

		if (info.find("name") != info.end()) {
			gameAudio->name = info["name"].get<std::string>();
		}

		if (info.find("volume") != info.end()) {
			gameAudio->volume = info["volume"].get<float>();
		}
		else {
			gameAudio->volume = 1.0f;
		}

		::fmod_system->createChannelGroup(gameAudio->name.c_str(), &gameAudio->channel_group);

		if (info.find("position") != info.end()) {
			float x = info["position"][0].get<float>();
			float y = info["position"][1].get<float>();
			float z = info["position"][2].get<float>();
			gameAudio->position = glm::vec3(x, y, z);
		}

		if (info.find("audios") != info.end()) {

			for (json::iterator audio = info.find("audios")->begin();
				audio != info.find("audios")->end(); audio++)
			{
				AudioItem* ai = new AudioItem(::fmod_system);
				ai->path = audio->get<std::string>();
				ai->channel_group = gameAudio->channel_group;
				ai->create_and_play_3d_sound(false, glm_2_fmod_vec(gameAudio->position));
				gameAudio->audios.push_back(ai);
			}
		}

		error_check(gameAudio->channel_group->setVolume(gameAudio->volume));

		gameAudio->applyDSPs();

		return gameAudio;
	}
	else {
		printf("Unrecognized GameItem %s\n", type.c_str());
		return NULL;
	}
}