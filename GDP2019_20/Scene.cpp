#include "GLCommon.h"
#include "Scene.h"
#include "cModelLoader.h"
#include "globalStuff.h"

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
	pLargeBunny->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pLargeBunny->physicsShapeType = MESH;
	pLargeBunny->inverseMass = 0.0f;	// Ignored during update
	addGameObject(pLargeBunny);

	cGameObject* pTerrain = new cGameObject();			// HEAP
	pTerrain->meshName = "terrain";
	pTerrain->friendlyName = "TheGround";
	pTerrain->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pTerrain->rotationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	pTerrain->scale = 1.0f;
	pTerrain->objectColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
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
	light->specularColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);;
	light->isOn = true;
	// Add it to the map
	lights["light1"] = light;
}

void Scene::loadScene(std::string filename) {
	loadObjects(filename);
	loadLights(filename);
}

void Scene::drawObjects()
{
	for (std::map<std::string, cGameObject*>::iterator i = game_objects.begin();
		i != game_objects.end(); i++)
	{
		cGameObject* pCurrentObject = i->second;

		glm::mat4 m = glm::mat4(1.0f);

		// ******* TRANSLATION TRANSFORM *********
		glm::mat4 matTrans
			= glm::translate(glm::mat4(1.0f),
				glm::vec3(pCurrentObject->positionXYZ.x,
					pCurrentObject->positionXYZ.y,
					pCurrentObject->positionXYZ.z));
		m = m * matTrans;

		// ******* ROTATION TRANSFORM *********
		glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
			pCurrentObject->rotationXYZ.z,	// Angle 
			glm::vec3(0.0f, 0.0f, 1.0f));
		m = m * rotateZ;

		glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
			pCurrentObject->rotationXYZ.y,
			glm::vec3(0.0f, 1.0f, 0.0f));
		m = m * rotateY;

		glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
			pCurrentObject->rotationXYZ.x,
			glm::vec3(1.0f, 0.0f, 0.0f));
		m = m * rotateX;

		// ******* SCALE TRANSFORM *********
		glm::mat4 scale = glm::scale(glm::mat4(1.0f),
			glm::vec3(pCurrentObject->scale,
				pCurrentObject->scale,
				pCurrentObject->scale));
		m = m * scale;

		GLuint programID = ::theShaderManager.getIDFromFriendlyName(::shader_name);


		GLint matModel_UL = glGetUniformLocation(programID, "matModel");
		glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(m));

		// Calcualte the inverse transpose of the model matrix and pass that...
		// Stripping away scaling and translation, leaving only rotation
		// Because the normal is only a direction, really
		GLint matModelIT_UL = glGetUniformLocation(programID, "matModelInverseTranspose");
		glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(m));
		glUniformMatrix4fv(matModelIT_UL, 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));

		// Find the location of the uniform variable newColour
		GLint newColour_location = glGetUniformLocation(programID, "newColour");

		glUniform3f(newColour_location,
			pCurrentObject->objectColourRGBA.r,
			pCurrentObject->objectColourRGBA.g,
			pCurrentObject->objectColourRGBA.b);

		GLint diffuseColour_UL = glGetUniformLocation(programID, "diffuseColour");
		glUniform4f(diffuseColour_UL,
			pCurrentObject->objectColourRGBA.r,
			pCurrentObject->objectColourRGBA.g,
			pCurrentObject->objectColourRGBA.b,
			pCurrentObject->objectColourRGBA.a);

		GLint specularColour_UL = glGetUniformLocation(programID, "specularColour");
		glUniform4f(specularColour_UL,
			1.0f,	// R
			1.0f,	// G
			1.0f,	// B
			1000.0f);	// Specular "power" (how shinny the object is)
						// 1.0 to really big (10000.0f)

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID

		if (pCurrentObject->disableDepthBufferTest)
		{
			glDisable(GL_DEPTH_TEST);					// DEPTH Test OFF
		}
		else
		{
			glEnable(GL_DEPTH_TEST);						// Turn ON depth test
		}

		if (pCurrentObject->disableDepthBufferWrite)
		{
			glDisable(GL_DEPTH);						// DON'T Write to depth buffer
		}
		else
		{
			glEnable(GL_DEPTH);								// Write to depth buffer
		}

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