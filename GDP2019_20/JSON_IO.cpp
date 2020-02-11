#include "JSON_IO.h"
#include "globalStuff.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <glm/vec4.hpp> // glm::vec4

#include "cMesh.h"
#include "GameItemFactory/GameItemFactory.h"
#include "RenderManager.h"
#include <iostream>
#include "SceneEditor.h"

using json = nlohmann::json;

json loaded_textures;

bool jsonContains(json j, std::string k)
{
	return j.find(k) != j.end();
}

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

	i.close();

	return vMeshes;
}

bool readTextures(std::string filename) {
	std::ifstream i;
	i.open(filename);

	if (!i.is_open()) {
		printf("Didn't found %s file \n", filename.c_str());
		return false;
	}

	json jFile;
	i >> jFile;

	json::iterator jTextures = jFile.find("Textures");

	if (jTextures == jFile.end()) {
		printf("No textures\n");
		i.close();
		return true;
	}

	loaded_textures = (*jTextures);

	::g_pTextureManager->SetBasePath("assets/textures");

	for (json::iterator itTexture = jTextures->begin();
		itTexture != jTextures->end(); itTexture++) {
		if (!::g_pTextureManager->Create2DTextureFromBMPFile((*itTexture).get<std::string>(), true)) {
			printf("error while reading texture %s\n", (*itTexture).get<std::string>().c_str());
			i.close();
			return false;
		}
	}

	i.close();

	return true;
}

std::map<std::string, sCameraSettings*>* readCameras(std::string filename) {
	std::ifstream i;
	i.open(filename);

	if (!i.is_open()) {
		printf("Didn't found %s file \n", filename.c_str());
		return NULL;
	}

	json jFile;
	i >> jFile;

	std::map<std::string, sCameraSettings*>* mCameras = new std::map<std::string, sCameraSettings*>();

	json::iterator jCameras = jFile.find("Cameras");
	if (jCameras == jFile.end()) {
		printf("No Cameras found!!\n");
	}
	else 
	{
		for (json::iterator itCameras = jCameras->begin();
			itCameras != jCameras->end(); itCameras++) {

			sCameraSettings* cameraSetting = new sCameraSettings();
			if (itCameras->find("name") == itCameras->end()) {
				printf("Camera without name!!\n");
				continue;
			}
			cameraSetting->name = (*itCameras)["name"].get<std::string>();

			if (itCameras->find("position") != itCameras->end()) {
				cameraSetting->position.x = (*itCameras)["position"][0].get<float>();
				cameraSetting->position.y = (*itCameras)["position"][1].get<float>();
				cameraSetting->position.z = (*itCameras)["position"][2].get<float>();
			}
			else {
				cameraSetting->position = glm::vec3(1.0f) * 50.0f;
			}

			if (itCameras->find("target") != itCameras->end()) {
				cameraSetting->target.x = (*itCameras)["target"][0].get<float>();
				cameraSetting->target.y = (*itCameras)["target"][1].get<float>();
				cameraSetting->target.z = (*itCameras)["target"][2].get<float>();
			}
			else {
				cameraSetting->target = glm::vec3(0.0f);
			}

			(*mCameras)[cameraSetting->name] = cameraSetting;
		}
	}

	i.close();

	return mCameras;
}

std::map<std::string, aGameItem*>* readItems(std::string filename) {
	std::ifstream i;
	i.open(filename);

	if (!i.is_open()) {
		printf("Didn't found %s file \n", filename.c_str());
		return NULL;
	}

	json jFile;
	i >> jFile;

	std::map<std::string, aGameItem*>* mItems = new std::map<std::string, aGameItem*>();

	// create GameObjects
	json::iterator jObjects = jFile.find("Objects");
	if (jObjects == jFile.end()) {
		printf("No Objects found!!\n");
	}
	else {	
		for (json::iterator jObj = jObjects->begin();
			jObj != jObjects->end(); jObj++) {

			aGameItem* gameItem = createGameItem("Object", *jObj);


			(*mItems)[gameItem->getName()] = gameItem;
		}
	}

	// create Lights
	json::iterator jLights = jFile.find("Lights");
	if (jLights == jFile.end()) {
		printf("No Lights found!!\n");
	}
	else {
		resetFactory("Light");
		for (json::iterator jLight = jLights->begin();
			jLight != jLights->end(); jLight++) {

			aGameItem* gameItem = createGameItem("Light", *jLight);

			(*mItems)[gameItem->getName()] = gameItem;
		}
	}

	// create Sounds
	json::iterator jAudios = jFile.find("Sounds");
	if (jAudios == jFile.end()) {
		printf("No Sounds found!!\n");
	}
	else {
		for (json::iterator jAudio = jAudios->begin();
			jAudio != jAudios->end(); jAudio++) {

			aGameItem* gameItem = createGameItem("sound", *jAudio);

			(*mItems)[gameItem->getName()] = gameItem;
		}
	}

	return mItems;
}

json serializeMeshes(std::map<std::string, cMesh*> meshes) {
	json jvMeshes;
	int i;
	std::map<std::string, cMesh*>::iterator iMesh;
	for (iMesh = meshes.begin(), i = 0;
		iMesh != meshes.end(); iMesh++, i++) {

		cMesh* mesh = iMesh->second;
		json jMesh;

		jMesh["name"] = iMesh->first;
		jMesh["filename"] = mesh->filename;

		jvMeshes[i] = jMesh;
	}

	return jvMeshes;
}

json serializeCameras(std::map<std::string, sCameraSettings*> cameras) {
	json jCameras;
	int i;
	std::map<std::string, sCameraSettings*>::iterator itCameras;
	for (itCameras = cameras.begin(), i = 0;
		itCameras != cameras.end(); itCameras++, i++) {

		sCameraSettings* camera = itCameras->second;
		json jCamera;

		jCamera["name"] = camera->name;
		jCamera["position"][0] = camera->position.x;
		jCamera["position"][1] = camera->position.y;
		jCamera["position"][2] = camera->position.z;
		jCamera["target"][0] = camera->target.x;
		jCamera["target"][1] = camera->target.y;
		jCamera["target"][2] = camera->target.z;

		jCameras[i] = jCamera;
	}

	return jCameras;
}

void saveScene(Scene* scene, std::string filename) {
	std::vector<aGameItem*> items = scene->getItems();

	json jObjs, jLights, jSounds, jScene;

	for (int i = 0, light = 0, sound = 0, obj = 0; i < items.size(); i++) {
		aGameItem* item = items[i];

		if (item->getType() == "Object") {
			jObjs[obj] = item->toJSON();
			obj++;
		}
		if (item->getType() == "Light") {
			jLights[light] = item->toJSON();
			light++;
		}
		if (item->getType() == "audio") {
			jSounds[sound] = item->toJSON();
			sound++;
		}
	}

	jScene["Lights"] = jLights; jScene["Objects"] = jObjs; jScene["Sounds"] = jSounds;
	jScene["Meshes"] = serializeMeshes(scene->getMeshesMap());
	jScene["Cameras"] = serializeCameras(scene->getCamerasMap());
	jScene["Textures"] = loaded_textures;

	if (scene->pSkyBox)
	{
		jScene["SkyBox"]["front"] = scene->pSkyBox->defs.front;
		jScene["SkyBox"]["back"] = scene->pSkyBox->defs.back;
		jScene["SkyBox"]["left"] = scene->pSkyBox->defs.left;
		jScene["SkyBox"]["right"] = scene->pSkyBox->defs.right;
		jScene["SkyBox"]["top"] = scene->pSkyBox->defs.top;
		jScene["SkyBox"]["bottom"] = scene->pSkyBox->defs.bottom;
	}

	std::ofstream file(filename);

	file << jScene;

	file.close();

	std::cout << filename << " succesfully saved\n";
}

bool loadScenes(std::string filename)
{
	std::ifstream i;
	i.open(filename);

	if (!i.is_open())
	{
		printf("\"loadScenes\" Didn't found %s file \n", filename.c_str());
		return false;
	}

	json jFile;
	i >> jFile;

	json::iterator jScenes = jFile.find("Scenes");
	if (!jsonContains(jFile, "Scenes"))
	{
		printf("No Scenes found!!\n");
		return false;
	}

	RenderManager::mScenes.clear();
	for (const auto &jScene : jFile["Scenes"])
	{
		bool isRendered = jsonContains(jScene, "isRendered") ? jScene["isRendered"].get<bool>() : true;

		if (!isRendered) { continue; }
		
		if (!jsonContains(jScene,"name"))
		{
			printf("Scene without name!!\n");
			return false;
		}
		
		if (!jsonContains(jScene, "path"))
		{
			printf("Scene without path file!!\n");
			return false;
		}

		std::string scenePath = jScene["path"].get<std::string>();
		std::string sceneName = jScene["name"].get<std::string>();
		int width = jsonContains(jScene, "width") ? jScene["width"].get<int>() : 1600;
		int height = jsonContains(jScene, "height") ? jScene["height"].get<int>() : 800;
		bool isDeferred = jsonContains(jScene, "isDeferred") ? jScene["isDeferred"].get<bool>() : false;
		bool isEdited = jsonContains(jScene, "isEdited") ? jScene["isEdited"].get<bool>() : false;

		cFBO* pFBO = nullptr;
		if (isDeferred)
		{
			pFBO = new cFBO();
			std::string fboError = "";
			if (!pFBO->init(width, height, fboError))
			{
				std::cout << "FBO error in " << sceneName <<
					" scene: " << fboError << "\n";
				delete pFBO;
				return false;
			}
		}
		
		readTextures(scenePath);
		auto pScene = new Scene();
		pScene->loadScene(scenePath);		
		auto pSceneDefs = new SceneDefs();
		pSceneDefs->pFBO = pFBO;
		pSceneDefs->width = width;
		pSceneDefs->height = height;
		pSceneDefs->pScene = pScene;
		pSceneDefs->name = sceneName;
		pSceneDefs->pathfile = scenePath;

		if (isEdited)
		{
			SceneEditor::getTheEditor()->init(pScene);
			RenderManager::sceneOnEdition = pSceneDefs;
		}
		
		RenderManager::mScenes[sceneName] = pSceneDefs;
	}
	return true;
}

bool readSkybox(std::string filename, SkyBox* &skybox)
{
	std::ifstream i;
	i.open(filename);

	if (!i.is_open())
	{
		printf("\"loadSkybox\" Didn't found %s file \n", filename.c_str());
		return false;
	}

	json jFile;
	i >> jFile;

	if (!jsonContains(jFile, "SkyBox"))
	{
		printf("No Skybox settings found.\n");
		skybox = nullptr;
		return true;
	}

	json jSkybox = jFile["SkyBox"];
	if (skybox != nullptr)
		delete skybox;
	skybox = new SkyBox();
	return skybox->init(
		jSkybox["right"].get<std::string>(),
		jSkybox["left"].get<std::string>(),
		jSkybox["top"].get<std::string>(),
		jSkybox["bottom"].get<std::string>(),
		jSkybox["front"].get<std::string>(),
		jSkybox["back"].get<std::string>(),
		"sphere_model"
	);
}