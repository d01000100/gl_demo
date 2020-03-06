#include "../globalStuff.h"

#include "GameItemFactory.h"
#include "iGameItem.h"
#include "../cGameObject.h"
#include "../cLight.h"
#include "../PhysicsConfigs.h"
#include <iostream>

using namespace nlohmann;

iPhysicsComponent* buildPhysicsObject(json jPhysics) {

	if (jPhysics.find("shape") == jPhysics.end()) {
		std::cout << "Physics Object without shape!!\n";
		return nullptr;
	}
	else if (jPhysics["shape"] == "sphere")
	{
		sBallDef ballDefs;
		if (jPhysics.find("position") != jPhysics.end()) {
			float x = jPhysics["position"][0].get<float>();
			float y = jPhysics["position"][1].get<float>();
			float z = jPhysics["position"][2].get<float>();
			ballDefs.Position = glm::vec3(x, y, z);
		}
		if (jPhysics.find("radius") != jPhysics.end()) {
			ballDefs.Radius = jPhysics["radius"].get<float>();
		}
		if (jPhysics.find("mass") != jPhysics.end()) {
			ballDefs.Mass = jPhysics["mass"].get<float>();
		}
		return ::g_PhysicsFactory->CreateBall(ballDefs);
	}
	else if (jPhysics["shape"] == "plane")
	{
		sPlaneDef planeDefs;
		if (jPhysics.find("position") != jPhysics.end()) {
			float x = jPhysics["position"][0].get<float>();
			float y = jPhysics["position"][1].get<float>();
			float z = jPhysics["position"][2].get<float>();
			planeDefs.Point = glm::vec3(x, y, z);
		}
		if (jPhysics.find("normal") != jPhysics.end()) {
			float x = jPhysics["normal"][0].get<float>();
			float y = jPhysics["normal"][1].get<float>();
			float z = jPhysics["normal"][2].get<float>();
			planeDefs.Normal = glm::normalize(glm::vec3(x, y, z));
		}
		return ::g_PhysicsFactory->CreatePlane(planeDefs);
	}
	else if (jPhysics["shape"] == "cloth")
	{
		sClothDef clothDefs;
		if (jPhysics.find("CornerA") != jPhysics.end())
		{
			float x = jPhysics["CornerA"][0].get<float>();
			float y = jPhysics["CornerA"][1].get<float>();
			float z = jPhysics["CornerA"][2].get<float>();
			clothDefs.CornerA = glm::vec3(x, y, z);
		}
		if (jPhysics.find("CornerB") != jPhysics.end())
		{
			float x = jPhysics["CornerB"][0].get<float>();
			float y = jPhysics["CornerB"][1].get<float>();
			float z = jPhysics["CornerB"][2].get<float>();
			clothDefs.CornerB = glm::vec3(x, y, z);
		}
		if (jPhysics.find("DownDirection") != jPhysics.end())
		{
			float x = jPhysics["DownDirection"][0].get<float>();
			float y = jPhysics["DownDirection"][1].get<float>();
			float z = jPhysics["DownDirection"][2].get<float>();
			clothDefs.DownDirection = glm::vec3(x, y, z);
		}
		if (jPhysics.find("NumNodesAcross") != jPhysics.end())
		{
			clothDefs.NumNodesAcross = jPhysics["NumNodesAcross"];
		}
		if (jPhysics.find("NumNodesDown") != jPhysics.end())
		{
			clothDefs.NumNodesDown = jPhysics["NumNodesDown"];
		}
		if (jPhysics.find("mass") != jPhysics.end())
		{
			clothDefs.NodeMass = jPhysics["mass"];
		}
		if (jPhysics.find("SpringConstant") != jPhysics.end())
		{
			clothDefs.SpringConstant = jPhysics["SpringConstant"];
		}
		return ::g_PhysicsFactory->CreateCloth(clothDefs);
	}
	else
	{
		std::cout << "Unsopported physics shape while building physics component.\n";
		return nullptr;
	}
}

aGameItem* createGameItem(std::string type, json info) {
	if (type == "Object") {
		cGameObject* gameObj = new cGameObject();

		if (info.find("mesh") == info.end()) {
			printf("Object without mesh!!\n");
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
			float x = info["rotation"][0].get<float>();
			float y = info["rotation"][1].get<float>();
			float z = info["rotation"][2].get<float>();
			gameObj->setOrientation(glm::vec3(x, y, z));
		}
		else {
			gameObj->setOrientation(glm::vec3(0));
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
			gameObj->diffuseColor = Colors::grey;
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

		if (info.find("isLit") != info.end()) {
			gameObj->isLit = info["isLit"].get<bool>();
		}
		else {
			gameObj->isLit = true;
		}

		if (info.find("alpha") != info.end()) {
			gameObj->alpha = info["alpha"].get<float>();
		}

		if (info.find("Physics") != info.end()) {
			gameObj->mPhysicsCompoment = buildPhysicsObject(info["Physics"]);
			::g_PhysicsWorld->AddComponent(gameObj->mPhysicsCompoment);
		}

		if (info.find("textures") != info.end()) {
			for (json::iterator itJText = info["textures"].begin();
				itJText != info["textures"].end(); itJText++) 
			{
				sTextureSpec textSpec;

				if (gameObj->textures.size() == 4) {
					printf("%s already has 4 textures\n", gameObj->friendlyName.c_str());
					break;
				}

				if ((*itJText).find("textureName") != (*itJText).end()) {
					textSpec.textureName = (*itJText)["textureName"].get<std::string>();

					if ((*itJText).find("weight") != (*itJText).end()) {
						textSpec.weight = (*itJText)["weight"].get<float>();
					}

					gameObj->textures.push_back(textSpec);
				}

			}
		}

		if (info.find("collision_points") != info.end()) {
			for (json::iterator itPoints = info["collision_points"].begin();
				itPoints != info["collision_points"].end(); itPoints++)
			{
				glm::vec3 point((*itPoints)[0], (*itPoints)[1], (*itPoints)[2]);
				gameObj->collision_points.push_back(point);
			}
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
			light->setDirection(glm::vec3(x, y, z));
		}
		else {
			light->setDirection(glm::vec3(0,-1,0));
		}

		if (info.find("innerAngle") != info.end()) {
			light->innerAngle = info["innerAngle"].get<float>();
		}

		if (info.find("outerAngle") != info.end()) {
			light->outerAngle = info["outerAngle"].get<float>();
		}

		return light;
	}
	else {
		printf("Unrecognized GameItem %s\n", type.c_str());
		return NULL;
	}
}

void resetFactory(std::string type) {
	if (type == "Light") { cLight::resetIds(); }
}