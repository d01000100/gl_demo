#include "SceneEditor.h"
#include "Camera.h"
#include "Scene.h"
#include <glm/gtx/rotate_vector.hpp>

SceneEditor::SceneEditor() {}

SceneEditor* SceneEditor::theEditor = new SceneEditor();

SceneEditor* SceneEditor::getTheEditor() {
	return theEditor;
}

void SceneEditor::init(Scene* scene) {
    //objects = scene->getGameObjects();
	selectedObj = objects.begin();
	lights = scene->getLights();
	selectedLight = lights.begin();
	editMode = OBJS;
	objectMode = TRANS;
	debugRenderer = new cDebugRenderer();
	debugRenderer->initialize();
	Camera* theCamera = Camera::getTheCamera();
	//theCamera->setTarget((*selectedObj)->position);
}

cDebugRenderer* SceneEditor::getDebugRenderer() {
	return debugRenderer;
}

void SceneEditor::drawDebug() {
	switch (editMode) {
	case OBJS:
		objectDebug();
		break;
	case LIGHTS:
		LightDebug();
		break;
	}
}

void SceneEditor::setEditMode(eEditMode m) {
	editMode = m;
	Camera* theCamera = Camera::getTheCamera();
	if (m == OBJS) {
		glm::vec3 objPos = (*selectedObj)->getPos();
		theCamera->setTarget(objPos);
		//theCamera->setPosition(objPos + glm::vec3(0.0f, 0.0f, -50.0f));
	}
	if (m == LIGHTS) {
		glm::vec3 objPos = (*selectedLight)->pos;
		theCamera->setTarget(objPos);
		//theCamera->setPosition(objPos + glm::vec3(0.0f, 0.0f, -50.0f));
	}
}

eEditMode SceneEditor::getEditMode() {
	return editMode;
}

void SceneEditor::nextObject() {
	selectedObj++;
	if (selectedObj == objects.end()) {
		selectedObj = objects.begin();
	}
	Camera* theCamera = Camera::getTheCamera();
	glm::vec3 objPos = (*selectedObj)->getPos();
	theCamera->setTarget(objPos);
	//theCamera->setPosition(objPos + glm::vec3(0.0f, 0.0f, -50.0f));
}

void SceneEditor::previousObject() {
	if (selectedObj == objects.begin()) {
		selectedObj = objects.end();
	}
	selectedObj--;
	Camera* theCamera = Camera::getTheCamera();
	glm::vec3 objPos = (*selectedObj)->getPos();
	theCamera->setTarget(objPos);
	//theCamera->setPosition(objPos + glm::vec3(0.0f, 0.0f, -50.0f));
}

void SceneEditor::setObjectMode(eObjectMode m) {
	objectMode = m;
}

void SceneEditor::translateObject(glm::vec3 deltaTranslation) {
	glm::vec3 pos;
	switch (editMode) {
	case OBJS:
		pos = (*selectedObj)->getPos();
		break;
	case LIGHTS:
		pos = (*selectedLight)->pos;
		break;
	}
	pos += deltaTranslation;
	Camera* theCamera = Camera::getTheCamera();
	theCamera->setTarget(pos);
	theCamera->setPosition(theCamera->getPosition() + deltaTranslation);

	switch (editMode) {
	case OBJS:
		//(*selectedObj)->position = pos;
		break;
	case LIGHTS:
		(*selectedLight)->pos = pos;
		break;
	}
}

void SceneEditor::rotateObject(glm::vec3 deltaRotation) {
	switch (editMode) {
	case OBJS:
		//(*selectedObj)->rotationXYZ += deltaRotation;
		break;
	case LIGHTS:
		(*selectedLight)->direction = glm::normalize((*selectedLight)->direction + deltaRotation);
		break;
	}
}

void SceneEditor::scaleObject(float deltaScale) {
	switch (editMode) {
	case OBJS:
		//(*selectedObj)->scale *= deltaScale;
		break;
	case LIGHTS:
		(*selectedLight)->linearAtten *= deltaScale;
		break;
	}
}

void SceneEditor::objectDebug() {
	glm::vec3 pos = (**selectedObj).getPos();
	switch (objectMode) {
	case TRANS:
		debugTranslation(pos);
		break;
	case ROT:
		debugRotation(pos);
		break;
	case SCALE:
		debugScale(pos);
		break;
	}
}

void SceneEditor::debugTranslation(glm::vec3 pos) {

	debugRenderer->addLine(pos, pos + glm::vec3(5.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), 0.1f);
	debugRenderer->addLine(pos, pos + glm::vec3(0.0, 5.0, 0.0), glm::vec3(0.0, 1.0, 0.0), 0.1f);
	debugRenderer->addLine(pos, pos + glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 1.0), 0.1f);
}

void SceneEditor::debugRotation(glm::vec3 pos) {

	drLine xpos, xneg, zpos, zneg;
	glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.0f);

	xpos.points[0] = pos + glm::vec3(5.0f, 0.0f, 5.0f);
	xpos.points[1] = pos + glm::vec3(5.0f, 0.0f, -5.0f);
	xpos.colour = blue;

	xneg.points[0] = pos + glm::vec3(-5.0f, 0.0f, 5.0f);
	xneg.points[1] = pos + glm::vec3(-5.0f, 0.0f, -5.0f);
	xneg.colour = blue;

	zpos.points[0] = pos + glm::vec3(5.0f, 0.0f, 5.0f);
	zpos.points[1] = pos + glm::vec3(-5.0f, 0.0f, 5.0f);
	zpos.colour = yellow;

	zneg.points[0] = pos + glm::vec3(5.0f, 0.0f, -5.0f);
	zneg.points[1] = pos + glm::vec3(-5.0f, 0.0f, -5.0f);
	zneg.colour = yellow;

	debugRenderer->addLine(xpos);
	debugRenderer->addLine(xneg);
	debugRenderer->addLine(zpos);
	debugRenderer->addLine(zneg);
}

void SceneEditor::debugScale(glm::vec3 pos) {

	glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.0f);
	glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 cyan = glm::vec3(0.0f, 1.0f, 1.0f);

	float innerDist = 3.0f;
	float outerDist = innerDist + 5.0f;

	debugRenderer->addLine(
		pos + glm::vec3(innerDist, innerDist, innerDist), 
		pos + glm::vec3(outerDist, outerDist, outerDist), 
		red, 0.1f);

	debugRenderer->addLine(
		pos + glm::vec3(innerDist, innerDist, -innerDist),
		pos + glm::vec3(outerDist, outerDist, -outerDist),
		yellow, 0.1f);

	debugRenderer->addLine(
		pos + glm::vec3(innerDist, -innerDist, innerDist),
		pos + glm::vec3(outerDist, -outerDist, outerDist),
		blue, 0.1f);

	debugRenderer->addLine(
		pos + glm::vec3(innerDist, -innerDist, -innerDist),
		pos + glm::vec3(outerDist, -outerDist, -outerDist),
		cyan, 0.1f);

	debugRenderer->addLine(
		pos + glm::vec3(-innerDist, innerDist, innerDist),
		pos + glm::vec3(-outerDist, outerDist, outerDist),
		yellow, 0.1f);

	debugRenderer->addLine(
		pos + glm::vec3(-innerDist, innerDist, -innerDist),
		pos + glm::vec3(-outerDist, outerDist, -outerDist),
		blue, 0.1f);

	debugRenderer->addLine(
		pos + glm::vec3(-innerDist, -innerDist, innerDist),
		pos + glm::vec3(-outerDist, -outerDist, outerDist),
		cyan, 0.1f);

	debugRenderer->addLine(
		pos + glm::vec3(-innerDist, -innerDist, -innerDist),
		pos + glm::vec3(-outerDist, -outerDist, -outerDist),
		red, 0.1f);
}

void SceneEditor::nextLight() {
	selectedLight++;
	if (selectedLight == lights.end()) {
		selectedLight = lights.begin();
	}
	Camera* theCamera = Camera::getTheCamera();
	glm::vec3 LightPos = (*selectedLight)->pos;
	theCamera->setTarget(LightPos);
	//theCamera->setPosition(LightPos + glm::vec3(0.0f, 0.0f, -50.0f));
}

void SceneEditor::previousLight() {
	if (selectedLight == lights.begin()) {
		selectedLight = lights.end();
	}
	selectedLight--;
	Camera* theCamera = Camera::getTheCamera();
	glm::vec3 LightPos = (*selectedLight)->pos;
	theCamera->setTarget(LightPos);
	//theCamera->setPosition(LightPos + glm::vec3(0.0f, 0.0f, -50.0f));
}

void SceneEditor::translateLight(glm::vec3 deltaTranslation){ }
void SceneEditor::rotateLight(glm::vec3 deltaRotation){ }
void SceneEditor::changeLinearAtten(float deltaScale){ 
}
void SceneEditor::changeQuadAtten(float deltaScale) { 
	(*selectedLight)->quadAtten *= deltaScale;
}
void SceneEditor::changeInnerAngle(float deltaScale) {
	(*selectedLight)->innerAngle *= deltaScale;
}
void SceneEditor::changeOuterAngle(float deltaScale){ 
	(*selectedLight)->outerAngle *= deltaScale;
}

void SceneEditor::toggleLight() {
	(*selectedLight)->isOn = !(*selectedLight)->isOn; 
}

void SceneEditor::LightDebug(){ 
	glm::vec3 pos  = (*selectedLight)->pos;
	glm::vec3 dir = (*selectedLight)->direction;
	switch (objectMode) {
	case TRANS: 
		debugTranslation(pos);
		break;
	case ROT:
		debugRenderer->addLine(pos, pos + dir * 10.0f, glm::vec3(1.0, 1.0, 1.0), 0.1f);
		break;
	case SCALE:		
		debugScale(pos);
		break;
	case ANGLES:
		glm::vec3 leftInner = glm::rotate(dir, (*selectedLight)->innerAngle * 0.5f, glm::vec3(0.0, 1.0f, 0.0f));
		glm::vec3 rightInner = glm::rotate(dir, (*selectedLight)->innerAngle * -0.5f, glm::vec3(0.0, 1.0f, 0.0f));
		debugRenderer->addLine(pos, pos + leftInner * 10.0f, glm::vec3(1.0, 1.0, 1.0), 0.1f);
		debugRenderer->addLine(pos, pos + rightInner * 10.0f, glm::vec3(1.0, 1.0, 1.0), 0.1f);
		break;
	}
}