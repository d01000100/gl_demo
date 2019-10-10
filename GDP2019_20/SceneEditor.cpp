#include "SceneEditor.h"
#include "Camera.h"

SceneEditor::SceneEditor() {}

SceneEditor* SceneEditor::theEditor = new SceneEditor();

SceneEditor* SceneEditor::getTheEditor() {
	return theEditor;
}

void SceneEditor::init(std::vector<cGameObject*> objs) {
    objects = objs;
	selectedObj = objects.begin();
	objectMode = TRANS;
	debugRenderer = new cDebugRenderer();
	debugRenderer->initialize();
	Camera* theCamera = Camera::getTheCamera();
	theCamera->setTarget((*selectedObj)->position);
}

cDebugRenderer* SceneEditor::getDebugRenderer() {
	return debugRenderer;
}

void SceneEditor::nextObject() {
	selectedObj++;
	if (selectedObj == objects.end()) {
		selectedObj = objects.begin();
	}
	Camera* theCamera = Camera::getTheCamera();
	glm::vec3 objPos = (*selectedObj)->position;
	theCamera->setTarget(objPos);
	theCamera->setPosition(objPos + glm::vec3(0.0f, 0.0f, -50.0f));
}

void SceneEditor::previousObject() {
	if (selectedObj == objects.begin()) {
		selectedObj = objects.end();
	}
	selectedObj--;
	Camera* theCamera = Camera::getTheCamera();
	glm::vec3 objPos = (*selectedObj)->position;
	theCamera->setTarget(objPos);
	theCamera->setPosition(objPos + glm::vec3(0.0f, 0.0f, -50.0f));
}

void SceneEditor::setObjectMode(eObjectMode m) {
	objectMode = m;
}

void SceneEditor::translateObject(glm::vec3 deltaTranslation) {
	(*selectedObj)->position += deltaTranslation;
	glm::vec3 objPos = (*selectedObj)->position;
	Camera* theCamera = Camera::getTheCamera();
	theCamera->setTarget(objPos);
	theCamera->setPosition(theCamera->getPosition() + deltaTranslation);
}

void SceneEditor::rotateObject(glm::vec3 deltaRotation) {
	(*selectedObj)->rotationXYZ += deltaRotation;
}

void SceneEditor::scaleObject(float deltaScale) {
	(*selectedObj)->scale *= deltaScale;
}

void SceneEditor::objectDebug() {
	switch (objectMode) {
	case TRANS:
		objectDebugTranslation();
		break;
	case ROT:
		objectDebugRotation();
		break;
	case SCALE:
		objectDebugScale();
		break;
	}
}

void SceneEditor::objectDebugTranslation() {
	cGameObject obj = **selectedObj;
	glm::vec3 pos = obj.position;

	debugRenderer->addLine(pos, pos + glm::vec3(5.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), 0.1f);
	debugRenderer->addLine(pos, pos + glm::vec3(0.0, 5.0, 0.0), glm::vec3(0.0, 1.0, 0.0), 0.1f);
	debugRenderer->addLine(pos, pos + glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 1.0), 0.1f);
}

void SceneEditor::objectDebugRotation() {
	glm::vec3 pos = (*selectedObj)->position;

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

void SceneEditor::objectDebugScale() {
	glm::vec3 pos = (*selectedObj)->position;

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