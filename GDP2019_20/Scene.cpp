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

Scene::Scene() {
	theVAOManager = new cVAOManager();
}

Scene::~Scene() {
	delete theVAOManager;
}

Scene* Scene::getTheScene() {
	return theScene;
}

void Scene::addGameObject(cGameObject* obj) {
	game_objects[obj->friendlyName] = obj;
}

std::vector<cGameObject*> Scene::getGameObjects() {

	std::vector<cGameObject*> vs;
	for (std::map<std::string, cGameObject*>::iterator i = game_objects.begin();
		i != game_objects.end(); i++)
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

cGameObject* Scene::findGameObject(std::string name) {

	if (game_objects.find(name) != game_objects.end()) {
		return game_objects[name];
	}
	else {
		return NULL;
	}
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
		theVAOManager->LoadModelIntoVAO(settings.name,
			*data,
			drawInfo,
			programID);

		meshes[settings.name] = data;
	}

	return true;
}

bool Scene::loadObjects(std::string filename) {

	std::map<std::string, cGameObject*> *read_objects = readObjects(filename);
	if (read_objects == NULL) {
		return false;
	}

	for (std::map<std::string, cGameObject*>::iterator iObj = read_objects->begin();
		iObj != read_objects->end(); iObj++) {
		cGameObject* pObj = iObj->second;
		if (pObj->physics && pObj->physics->shape == MESH) {
			pObj->physics->mesh = meshes[pObj->meshName];
		}
	}

	game_objects = *read_objects;

	return true;
}

bool Scene::loadLights(std::string filename) {

	std::map<std::string, cLight*>* read_lights = readLights(filename);

	if (!read_lights) {	return false; }

	lights = *read_lights;
}

bool Scene::loadScene(std::string filename) {
	if (!loadMeshes(filename)) { return false; }
	if (!loadObjects(filename)) { return false; }
	if (!loadLights(filename)) { return false; }
	return true;
}

bool Scene::reloadScene(std::string filename) {
	if (!loadObjects(filename)) { return false; }
	if (!loadLights(filename)) { return false; }
	return true;
}

void Scene::drawObjects()
{
	for (std::map<std::string, cGameObject*>::iterator i = game_objects.begin();
		i != game_objects.end(); i++)
	{
		cGameObject* pCurrentObject = i->second;

		GLuint programID = ::theShaderManager.getIDFromFriendlyName(::shader_name);

		glm::mat4 m = pCurrentObject->calculateTransformationMatrix();

		GLint matModel_UL = glGetUniformLocation(programID, "matModel");
		glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(m));

		// Calcualte the inverse transpose of the model matrix and pass that...
		// Stripping away scaling and translation, leaving only rotation
		// Because the normal is only a direction, really
		GLint matModelIT_UL = glGetUniformLocation(programID, "matModelInverseTranspose");
		glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(m));
		glUniformMatrix4fv(matModelIT_UL, 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));

		GLint diffuseColour_UL = glGetUniformLocation(programID, "diffuseColour");
		glUniform4f(diffuseColour_UL,
			pCurrentObject->diffuseColor.r,
			pCurrentObject->diffuseColor.g,
			pCurrentObject->diffuseColor.b,
			pCurrentObject->diffuseColor.a);

		GLint specularColour_UL = glGetUniformLocation(programID, "specularColour");
		glUniform4f(specularColour_UL,
			pCurrentObject->specularColor.r,
			pCurrentObject->specularColor.g,
			pCurrentObject->specularColor.b,
			1000.0f);	// Specular "power" (how shinny the object is)
						// 1.0 to really big (10000.0f)

		glPolygonMode(GL_FRONT_AND_BACK, 
			pCurrentObject->isWireframe ? GL_LINE : GL_FILL);
		glEnable(GL_DEPTH_TEST);						// Turn ON depth test
		glEnable(GL_DEPTH);								// Write to depth buffer
		
		sModelDrawInfo drawInfo;
		if (theVAOManager->FindDrawInfoByModelName(pCurrentObject->meshName, drawInfo))
		{
			glBindVertexArray(drawInfo.VAO_ID);
			glDrawElements(GL_TRIANGLES,
				drawInfo.numberOfIndices,
				GL_UNSIGNED_INT,
				0);
			glBindVertexArray(0);
		}
	}
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

void Scene::drawScene() {
	drawObjects();
	drawLights();
}

void Scene::saveScene(std::string filename) {

}