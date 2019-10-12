#pragma once
#include "cGameObject.h"
#include "cLight.h"
#include "Scene.h"
#include "DebugRenderer/cDebugRenderer.h"
#include <vector>

enum eObjectMode {
	TRANS, ROT, SCALE, ANGLES
};

enum eEditMode {
	OBJS, LIGHTS, NONE
};

class SceneEditor
{
private:
	std::vector<cGameObject*>::iterator selectedObj;
	std::vector<cGameObject*> objects;
	std::vector<cLight*>::iterator selectedLight;
	std::vector<cLight*> lights;
	cDebugRenderer* debugRenderer;
	static SceneEditor* theEditor;
	void debugTranslation(glm::vec3 pos);
	void debugRotation(glm::vec3 pos);
	void debugScale(glm::vec3 pos);
	SceneEditor();
	eEditMode editMode;
	void objectDebug();
	void LightDebug();

public:
	eObjectMode objectMode;
	static SceneEditor* getTheEditor();
	void init(Scene* scene);
	cDebugRenderer* getDebugRenderer();

	void setEditMode(eEditMode m);
	eEditMode getEditMode();
	void drawDebug();

	void setObjectMode(eObjectMode m);
	void nextObject();
	void previousObject();
	void translateObject(glm::vec3 deltaTranslation);
	void rotateObject(glm::vec3 deltaRotation);
	void scaleObject(float deltaScale);

	void nextLight();
	void previousLight();
	void translateLight(glm::vec3 deltaTranslation);
	void rotateLight(glm::vec3 deltaRotation);
	void changeLinearAtten(float deltaScale);
	void changeQuadAtten(float deltaScale);
	void changeInnerAngle(float deltaScale);
	void changeOuterAngle(float deltaScale);
	void toggleLight();
};

