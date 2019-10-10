#pragma once
#include "cGameObject.h"
#include "DebugRenderer/cDebugRenderer.h"
#include <vector>

enum eObjectMode {
	TRANS, ROT, SCALE
};

class SceneEditor
{
private:
	std::vector<cGameObject*>::iterator selectedObj;
	std::vector<cGameObject*> objects;
	cDebugRenderer* debugRenderer;
	static SceneEditor* theEditor;
	void objectDebugTranslation();
	void objectDebugRotation();
	void objectDebugScale();
public:
	eObjectMode objectMode;
	SceneEditor();
	static SceneEditor* getTheEditor();
	void init(std::vector<cGameObject*> objs);
	cDebugRenderer* getDebugRenderer();
	void setObjectMode(eObjectMode m);
	void nextObject();
	void previousObject();
	void translateObject(glm::vec3 deltaTranslation);
	void rotateObject(glm::vec3 deltaRotation);
	void scaleObject(float deltaScale);
	void objectDebug();
};

