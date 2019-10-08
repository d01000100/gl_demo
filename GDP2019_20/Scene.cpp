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

	for (int i = 0; i < vMeshes->size(); i++) {

		meshSettings settings = vMeshes->at(i);
		cMesh* data = new cMesh();
		if (!model_loader.LoadPlyModel(settings.filename, *data)) {
			printf("Couldn't load %s model file\n", settings.filename);
			return false;
		}
		meshes[settings.name] = data;
	}

	return true;
}

void Scene::loadObjects(std::string filename) {

	// Load meshes
	cModelLoader* model_loader = new cModelLoader();

	cMesh largeBunnyMesh;
	model_loader->LoadPlyModel("assets/models/Large_Physics_Bunny_XYZ_N.ply", largeBunnyMesh);

	cMesh terrainMesh;
	model_loader->LoadPlyModel("assets/models/Terrain_XYZ_n.ply", terrainMesh);

	GLuint programID = ::theShaderManager.getIDFromFriendlyName(::shader_name);

	// Move to MeshLoader (Manager)
	sModelDrawInfo drawInfoTerrain;
	theVAOManager->LoadModelIntoVAO("terrain",
		terrainMesh,
		drawInfoTerrain,
		programID);

	sModelDrawInfo largeBunnyDrawInfo;
	theVAOManager->LoadModelIntoVAO("large_bunny",
		largeBunnyMesh,		// Sphere mesh info
		largeBunnyDrawInfo,
		programID);

	// Create Game 	Objects
	// At this point, the model is loaded into the GPU
	cGameObject* pLargeBunny = new cGameObject();			// HEAP
	pLargeBunny->meshName = "large_bunny";
	pLargeBunny->friendlyName = "largeBunny";
	pLargeBunny->positionXYZ = glm::vec3(0.0f, 20.0f, 0.0f);
	pLargeBunny->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pLargeBunny->scale = 1.0f;
	pLargeBunny->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pLargeBunny->specularColour = glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f);
	pLargeBunny->physicsShapeType = MESH;
	pLargeBunny->inverseMass = 0.0f;	// Ignored during update
	addGameObject(pLargeBunny);

	cGameObject* pTerrain = new cGameObject();			// HEAP
	pTerrain->meshName = "terrain";
	pTerrain->friendlyName = "TheGround";
	pTerrain->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pTerrain->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pTerrain->scale = 1.0f;
	pTerrain->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pTerrain->specularColour = glm::vec4(0.0f, 1.0f, 0.0f, 1000.0f);
	pTerrain->physicsShapeType = MESH;
	pTerrain->inverseMass = 0.0f;	// Ignored during update
	pTerrain->isVisible = false;
	addGameObject(pTerrain);
}

void Scene::loadLights(std::string filename) {

	// Specify the lights info 
	cLight* light = new cLight();
	light->type = POINT;
	light->pos = glm::vec3(0.0f, 30.0f, 0.0f);
	light->const_atten = 0.0000001f;			// not really used (can turn off and on the light)
	light->linear_atten = 0.03f;
	light->quad_atten = 0.0000001f;
	light->diffuseColor = glm::vec3(1.0f, 0.0f, 1.0f);
	light->specularColor = glm::vec4(0.0f, 1.0f, 0.0f, 500.0f);;
	light->isOn = true;
	// Add it to the map
	lights["light1"] = light;
}

bool Scene::loadScene(std::string filename) {
	if (!loadMeshes(filename)) { return false; }
	loadObjects(filename);
	loadLights(filename);
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
			pCurrentObject->diffuseColour.r,
			pCurrentObject->diffuseColour.g,
			pCurrentObject->diffuseColour.b,
			pCurrentObject->diffuseColour.a);

		GLint specularColour_UL = glGetUniformLocation(programID, "specularColour");
		glUniform4f(specularColour_UL,
			pCurrentObject->specularColour.r,
			pCurrentObject->specularColour.g,
			pCurrentObject->specularColour.b,
			1000.0f);	// Specular "power" (how shinny the object is)
						// 1.0 to really big (10000.0f)

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
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
		glUniform4f(L_0_atten, light->const_atten,  
					light->linear_atten, 
					light->quad_atten,	
					light->cutOffDist);	

		// x = lightType, y = inner angle, z = outer angle, w = TBD
		// 0 = pointlight
		// 1 = spot light
		// 2 = directional light
		glUniform4f(L_0_param1, light->type /*POINT light*/, light->inner_angle, light->outer_angle, 1.0f);
		glUniform4f(L_0_param2, light->isOn /*Light is on*/, 0.0f, 0.0f, 1.0f);
	}
}

void Scene::drawScene() {
	drawObjects();
	drawLights();
}

void Scene::saveScene(std::string filename) {

}