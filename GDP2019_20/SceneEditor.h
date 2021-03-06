#pragma once

#include "cLight.h"
#include "Scene.h"
#include "DebugRenderer/cDebugRenderer.h"
#include "GameItemFactory/iGameItem.h"
#include "Messages/iMessagable.h"
#include <vector>

enum eObjectMode {
	TRANS, ROT, SCALE, ANGLES
};

enum eEditMode {
	OBJS, LIGHTS, NONE
};

class SceneEditor : public iMessagable
{
private:
	static SceneEditor* theEditor;
	SceneEditor();

	eObjectMode objectMode;
	eEditMode editMode;
	cDebugRenderer* debugRenderer;
	void printInfo();

	std::vector<aGameItem*>::iterator selectedObj;
	std::vector<aGameItem*> objects;
	void changeObject();
	void nextObject();
	void previousObject();
	std::vector<cLight*>::iterator selectedLight;
	std::vector<cLight*> lights;
	void nextLight();
	void previousLight();

	void setEditMode(eEditMode m);

	void debugTranslation(glm::vec3 pos);
	void debugRotation(glm::vec3 pos);
	void debugScale(glm::vec3 pos);
	void objectDebug();
public:
	static SceneEditor* getTheEditor();
	void init(Scene* scene);

	cDebugRenderer* getDebugRenderer();
	void addDebugMarkers();

	void recieveMessage(sMessage message);
};

