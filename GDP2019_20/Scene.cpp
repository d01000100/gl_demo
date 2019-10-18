#include "GLCommon.h"
#include "Scene.h"
#include "cModelLoader.h"
#include "globalStuff.h"
#include "JSON_IO.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

Scene* Scene::theScene = new Scene();

// does nothing. Need to "implement" it to make it private
Scene::Scene() { } 

Scene* Scene::getTheScene() {
	return theScene;
}

std::vector<iGameItem*> Scene::getItems() {
	std::vector<iGameItem*> vs;
	for (std::map<std::string, iGameItem*>::iterator i = gameItems.begin();
		i != gameItems.end(); i++)
	{
		vs.push_back(i->second);
	}
	return vs;
}

std::vector<cLight*> Scene::getLights() {

	std::vector<cLight*> vs;
	for (std::map<std::string, cLight*>::iterator i = lights.begin();
		i != lights.end(); i++)
	{
		vs.push_back(i->second);
	}
	return vs;
}

std::map<std::string, cLight*> Scene::getLightsMap() { return lights;  }

std::map<std::string, cMesh*> Scene::getMeshesMap() {
	return meshes;
}

bool Scene::loadMeshes(std::string filename) {
	std::vector<meshSettings>* vMeshes = readMeshes(filename);

	if (!vMeshes) { return false; }
	
	cModelLoader model_loader;

	GLuint programID = ::theShaderManager.getIDFromFriendlyName(::shader_name);

	for (int i = 0; i < vMeshes->size(); i++) {
		meshSettings settings = vMeshes->at(i);
		cMesh* data = new cMesh();

		data->filename = settings.filename;

		if (!model_loader.LoadPlyModel(settings.filename, *data)) {
			printf("Couldn't load %s model file\n", settings.filename.c_str());
			return false;
		}

		sModelDrawInfo drawInfo;
		::theVAOManager->LoadModelIntoVAO(settings.name,
			*data,
			drawInfo,
			programID);

		meshes[settings.name] = data;
	}

	return true;
}

bool Scene::loadLights(std::string filename) {

	//std::map<std::string, cLight*>* read_lights = readLights(filename);

	//if (!read_lights) {	return false; }

	//lights = *read_lights;

	// TODO: Patch while the ItemFactory doesn't have lights
	cLight* theLight = new cLight();
	theLight->pos = glm::vec3(10.0,
		50.0,
		10.0);
	theLight->diffuseColor = glm::vec3(1.0f);
	theLight->linearAtten = 0.01f;
	theLight->quadAtten = 0.00000000001f;
	theLight->specularColor = glm::vec4(1.0f, 1.0f, 1.0f, 50.0f);
	theLight->type = DIRECTIONAL;
	theLight->direction = glm::vec3(0.11157132685184479,
		-0.8131065368652344,
		-0.5713227987289429);
	lights["theLight"] = theLight;

	return true;
}

bool Scene::loadScene(std::string filename) {
	if (!loadMeshes(filename)) { return false; }

	if (!reloadScene(filename)) { return false; }
	return true;
}

// will not reload Meshes
bool Scene::reloadScene(std::string filename) {
	//if (!loadObjects(filename)) { return false; }

	std::map<std::string, iGameItem*>* loaded_items = readItems(filename);
	if (loaded_items == NULL) { return false; }
	else {
		gameItems = *loaded_items;
	}

	if (!loadLights(filename)) { return false; }
	return true;
}

void Scene::drawLights() {

	GLuint shaderProgID = ::theShaderManager.getIDFromFriendlyName(::shader_name);

	std::map<std::string, cLight*>::iterator iLight;
	int index;
	for (iLight = lights.begin(), index = 0; iLight != lights.end(); iLight++, index++) {

		// Set the lighting values for the shader.
		// See fragmentShader for documentation
		std::string prefix = "theLights[";
		GLint L_0_position = glGetUniformLocation(shaderProgID, (prefix + std::to_string(index) + "].position").c_str());
		GLint L_0_diffuse = glGetUniformLocation(shaderProgID, (prefix + std::to_string(index) + "].diffuse").c_str());
		GLint L_0_specular = glGetUniformLocation(shaderProgID, (prefix + std::to_string(index) + "].specular").c_str());
		GLint L_0_atten = glGetUniformLocation(shaderProgID, (prefix + std::to_string(index) + "].atten").c_str());
		GLint L_0_direction = glGetUniformLocation(shaderProgID, (prefix + std::to_string(index) + "].direction").c_str());
		GLint L_0_param1 = glGetUniformLocation(shaderProgID, (prefix + std::to_string(index) + "].param1").c_str());
		GLint L_0_param2 = glGetUniformLocation(shaderProgID, (prefix + std::to_string(index) + "].param2").c_str());

		cLight* light = iLight->second;

		glUniform4f(L_0_position,
			light->pos.x,
			light->pos.y,
			light->pos.z,
			1.0f);
		glUniform4f(L_0_diffuse, light->diffuseColor.r, light->diffuseColor.g, light->diffuseColor.b, 1.0f);	
		glUniform4f(L_0_specular, light->specularColor.r, light->specularColor.g, light->specularColor.b, light->specularColor.a);	
		glUniform4f(L_0_atten, light->constAtten,  
					light->linearAtten, 
					light->quadAtten,	
					light->cutOffDist);	

		// x = lightType, y = inner angle, z = outer angle, w = TBD
		// 0 = pointlight
		// 1 = spot light
		// 2 = directional light
		glUniform4f(L_0_param1, light->type /*POINT light*/, light->innerAngle, light->outerAngle, 1.0f);
		glUniform4f(L_0_param2, light->isOn /*Light is on*/, 0.0f, 0.0f, 1.0f);

		glUniform4f(L_0_direction, 
					light->direction.x,
					light->direction.y,
					light->direction.z,
					1.0f );	
	}
}

void Scene::drawItems() {
	for (std::map<std::string, iGameItem*>::iterator i = gameItems.begin();
		i != gameItems.end(); i++)
	{
		i->second->draw();
	}
}

void Scene::drawScene() {
	//drawObjects();
	drawItems();
	drawLights();
}

void Scene::saveScene(std::string filename) {

}

void Scene::IntegrationStep(float deltaTime) {
	for (std::map<std::string, iGameItem*>::iterator itItem = gameItems.begin();
		itItem != gameItems.end(); itItem++) {
		sMessage m; m.name = "integration step"; m.fValue = deltaTime;
		itItem->second->recieveMessage(m);
	}
}