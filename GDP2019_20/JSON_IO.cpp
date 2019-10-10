#include "JSON_IO.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

#include "cMesh.h"

using json = nlohmann::json;

std::vector<meshSettings>* readMeshes(std::string filename) {
	std::ifstream i;
	i.open(filename);

	if (!i.is_open()) {
		printf("Didn't found %s file \n", filename.c_str());
		return NULL;
	}

	json jFile;
	i >> jFile;

	json::iterator jMeshes = jFile.find("Meshes");
	if (jMeshes == jFile.end()) {
		printf("No Meshes found!!\n");
		return NULL;
	}

	std::vector <meshSettings> *vMeshes = new std::vector <meshSettings>();
	for (json::iterator mesh = jMeshes->begin();
		mesh != jMeshes->end(); mesh++) {

		meshSettings mSettings;
		if (mesh->find("name") == mesh->end()) {
			printf("Mesh without name!!\n");
			return NULL;
		}
		mSettings.name = (*mesh)["name"].get<std::string>();
		if (mesh->find("filename") == mesh->end()) {
			printf("Mesh without filename!!\n");
			return NULL;
		}
		mSettings.filename = (*mesh)["filename"].get<std::string>();
		vMeshes->push_back(mSettings);
	}

	return vMeshes;
}

std::map<std::string, cGameObject*>* readObjects(std::string filename) {
	std::ifstream i;
	i.open(filename);

	if (!i.is_open()) {
		printf("Didn't found %s file \n", filename.c_str());
		return NULL;
	}

	json jFile;
	i >> jFile;

	json::iterator jObjects = jFile.find("Objects");
	if (jObjects == jFile.end()) {
		printf("No Objects found!!\n");
		return NULL;
	}

	std::map<std::string, cGameObject*>* mObjects = new std::map<std::string, cGameObject*>();
	for (json::iterator jObj = jObjects->begin();
		jObj != jObjects->end(); jObj++) {

		cGameObject* gameObj = new cGameObject();
		
		if (jObj->find("mesh") == jObj->end()) {
			printf("Object without mesh!!\n");
			return NULL;
		}
		else {
			gameObj->meshName = (*jObj)["mesh"].get<std::string>();
		}

		if (jObj->find("name") != jObj->end()) {
			gameObj->friendlyName = (*jObj)["name"].get<std::string>();
		}
		else {
			gameObj->friendlyName = std::to_string(gameObj->getUniqueID());
		}

		if (jObj->find("position") != jObj->end()) {
			float x = (*jObj)["position"][0].get<float>();
			float y = (*jObj)["position"][1].get<float>();
			float z = (*jObj)["position"][2].get<float>();
			gameObj->position = glm::vec3(x, y, z);
		}
		else {
			gameObj->position = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		if (jObj->find("rotation") != jObj->end()) {
			float x = glm::radians((*jObj)["rotation"][0].get<float>());
			float y = glm::radians((*jObj)["rotation"][1].get<float>());
			float z = glm::radians((*jObj)["rotation"][2].get<float>());
			gameObj->rotationXYZ = glm::vec3(x, y, z);
		}
		else {
			gameObj->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		if (jObj->find("scale") != jObj->end()) {
			gameObj->scale = (*jObj)["scale"].get<float>();
		}
		else {
			gameObj->scale = 1.0f;
		}

		if (jObj->find("diffuseColor") != jObj->end()) {
			float x = (*jObj)["diffuseColor"][0].get<float>();
			float y = (*jObj)["diffuseColor"][1].get<float>();
			float z = (*jObj)["diffuseColor"][2].get<float>();
			gameObj->diffuseColor = glm::vec4(x, y, z, 1.0f);
		}
		else {
			gameObj->diffuseColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		}

		if (jObj->find("specularColor") != jObj->end()) {
			float x = (*jObj)["specularColor"][0].get<float>();
			float y = (*jObj)["specularColor"][1].get<float>();
			float z = (*jObj)["specularColor"][2].get<float>();
			float w = (*jObj)["specularColor"][3].get<float>();
			gameObj->specularColor = glm::vec4(x, y, z, w);
		}
		else {
			gameObj->specularColor = glm::vec4(1.0f, 1.0f, 1.0f, 50.0f);
		}

		if (jObj->find("isVisible") != jObj->end()) {
			gameObj->isVisible = (*jObj)["isVisible"].get<bool>();
		}
		else {
			gameObj->isVisible = true;
		}

		if (jObj->find("front") != jObj->end()) {
			float x = (*jObj)["front"][0].get<float>();
			float y = (*jObj)["front"][1].get<float>();
			float z = (*jObj)["front"][2].get<float>();
			gameObj->front = glm::vec3(x, y, z);
		}
		else {
			gameObj->front = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
		}
		
		if (jObj->find("Physics") != jObj->end()) {

			sPhysicsObject *physics = new sPhysicsObject();
			json jPhysics = (*jObj)["Physics"];

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

			gameObj->physics = physics;
		}

		(*mObjects)[gameObj->friendlyName] = gameObj;
	}

	return mObjects;
}

std::map<std::string, cLight*>* readLights(std::string filename) {
	std::ifstream i;
	i.open(filename);

	if (!i.is_open()) {
		printf("Didn't found %s file \n", filename.c_str());
		return NULL;
	}

	json jFile;
	i >> jFile;

	json::iterator jLights = jFile.find("Lights");
	if (jLights == jFile.end()) {
		printf("No Lights found!!\n");
		return NULL;
	}

	std::map<std::string, cLight*>* mLights = new std::map<std::string, cLight*>();
	for (json::iterator jLight = jLights->begin();
		jLight != jLights->end(); jLight++) {

		cLight* light = new cLight();
		std::string name;

		if (jLight->find("name") == jLight->end()) {
			printf("Light without a name!!\n");
			return NULL;
		}
		else {
			name = (*jLight)["name"].get<std::string>();
		}

		if (jLight->find("position") != jLight->end()) {
			float x = (*jLight)["position"][0].get<float>();
			float y = (*jLight)["position"][1].get<float>();
			float z = (*jLight)["position"][2].get<float>();
			light->pos = glm::vec3(x, y, z);
		}
		else {
			light->pos = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		if (jLight->find("diffuseColor") != jLight->end()) {
			float x = (*jLight)["diffuseColor"][0].get<float>();
			float y = (*jLight)["diffuseColor"][1].get<float>();
			float z = (*jLight)["diffuseColor"][2].get<float>();
			light->diffuseColor = glm::vec3(x, y, z);
		}
		else {
			light->diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
		}

		if (jLight->find("specularColor") != jLight->end()) {
			float x = (*jLight)["specularColor"][0].get<float>();
			float y = (*jLight)["specularColor"][1].get<float>();
			float z = (*jLight)["specularColor"][2].get<float>();
			float w = (*jLight)["specularColor"][3].get<float>();
			light->specularColor = glm::vec4(x, y, z, w);
		}
		else {
			light->specularColor = glm::vec4(1.0f, 1.0f, 1.0f, 50.0f);
		}

		if (jLight->find("linearAtten") != jLight->end()) {
			light->linearAtten = (*jLight)["linearAtten"].get<float>();
		}
		else {
			light->linearAtten = 0.05f;
		}

		if (jLight->find("quadAtten") != jLight->end()) {
			light->quadAtten = (*jLight)["quadAtten"].get<float>();
		}
		else {
			light->quadAtten = 1.0f;
		}

		if (jLight->find("cutOffDist") != jLight->end()) {
			light->cutOffDist = (*jLight)["cutOffDist"].get<float>();
		}
		else {
			light->cutOffDist = 50.0f;
		}

		if (jLight->find("type") != jLight->end()) {
			std::string type = (*jLight)["type"].get<std::string>();
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

		if (jLight->find("isOn") != jLight->end()) {
			light->isOn = (*jLight)["isOn"].get<bool>();
		}
		else {
			light->isOn = true;
		}

		if (jLight->find("direction") != jLight->end()) {
			float x = (*jLight)["direction"][0].get<float>();
			float y = (*jLight)["direction"][1].get<float>();
			float z = (*jLight)["direction"][2].get<float>();
			light->direction = glm::normalize(glm::vec3(x,y,z));
		}

		if (jLight->find("innerAngle") != jLight->end()) {
			light->innerAngle = (*jLight)["innerAngle"].get<float>();
		}

		if (jLight->find("outerAngle") != jLight->end()) {
			light->outerAngle = (*jLight)["outerAngle"].get<float>();
		}

		(*mLights)[name] = light;
	}

	return mLights;
}