#include "SceneEditor.h"
#include "Camera.h"
#include "Scene.h"
#include <glm/gtx/rotate_vector.hpp>
#include <sstream>
#include "globalStuff.h"

SceneEditor* SceneEditor::theEditor = new SceneEditor();

SceneEditor* SceneEditor::getTheEditor() {
	return theEditor;
}

SceneEditor::SceneEditor() {
	selectedObj = objects.end();
	debugRenderer = NULL;
}

void SceneEditor::init(Scene* scene) {
    objects = scene->getItems();
	selectedObj = objects.begin();
	objectMode = TRANS;
	editMode = OBJS;
	debugRenderer = new cDebugRenderer();
	debugRenderer->initialize();
	changeObject();
}

cDebugRenderer* SceneEditor::getDebugRenderer() {
	return debugRenderer;
}

void SceneEditor::addDebugMarkers() {
	objectDebug();
}

void SceneEditor::setEditMode(eEditMode m) {
	editMode = m;
	Camera* theCamera = Camera::getTheCamera();
	if (m == OBJS) {
		glm::vec3 objPos = (*selectedObj)->getPos();
		theCamera->setTarget(objPos);
		theCamera->setPosition(objPos + glm::vec3(0.0f, 0.0f, -50.0f));
	}
	if (m == LIGHTS) {
		glm::vec3 objPos = (*selectedLight)->pos;
		theCamera->setTarget(objPos);
		theCamera->setPosition(objPos + glm::vec3(0.0f, 0.0f, -50.0f));
	}
}

void SceneEditor::changeObject() {
	Camera* theCamera = Camera::getTheCamera();
	glm::vec3 objPos = (*selectedObj)->getPos();
	theCamera->setTarget(objPos);
}

void SceneEditor::nextObject() {
	selectedObj++;
	if (selectedObj == objects.end()) {
		selectedObj = objects.begin();
	}
	changeObject();
}

void SceneEditor::previousObject() {
	if (selectedObj == objects.begin()) {
		selectedObj = objects.end();
	}
	selectedObj--;
	changeObject();
}

void SceneEditor::objectDebug() {
	if (debugRenderer) {
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

		glfwSetWindowTitle(::window, (*selectedObj)->getInfo().c_str());
	}
}

void SceneEditor::debugTranslation(glm::vec3 pos) 
{
	debugRenderer->addLine(pos, pos + glm::vec3(5.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), 0.1f);
	debugRenderer->addLine(pos, pos + glm::vec3(0.0, 5.0, 0.0), glm::vec3(0.0, 1.0, 0.0), 0.1f);
	debugRenderer->addLine(pos, pos + glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 1.0), 0.1f);
}

void SceneEditor::debugRotation(glm::vec3 pos) {

	//drLine xpos, xneg, zpos, zneg;
	//glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
	//glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.0f);

	//xpos.points[0] = pos + glm::vec3(5.0f, 0.0f, 5.0f);
	//xpos.points[1] = pos + glm::vec3(5.0f, 0.0f, -5.0f);
	//xpos.colour = blue;

	//xneg.points[0] = pos + glm::vec3(-5.0f, 0.0f, 5.0f);
	//xneg.points[1] = pos + glm::vec3(-5.0f, 0.0f, -5.0f);
	//xneg.colour = blue;

	//zpos.points[0] = pos + glm::vec3(5.0f, 0.0f, 5.0f);
	//zpos.points[1] = pos + glm::vec3(-5.0f, 0.0f, 5.0f);
	//zpos.colour = yellow;

	//zneg.points[0] = pos + glm::vec3(5.0f, 0.0f, -5.0f);
	//zneg.points[1] = pos + glm::vec3(-5.0f, 0.0f, -5.0f);
	//zneg.colour = yellow;

	//debugRenderer->addLine(xpos);
	//debugRenderer->addLine(xneg);
	//debugRenderer->addLine(zpos);
	//debugRenderer->addLine(zneg);

	if (selectedObj != objects.end())
	{
		(*selectedObj)->debugOrientation();
	}
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
	theCamera->setPosition(LightPos + glm::vec3(0.0f, 0.0f, -50.0f));
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

void SceneEditor::printInfo() {
	for (std::vector<aGameItem*>::iterator i = objects.begin();
		i != objects.end(); i++)
	{
		printf("%s: %s\n", (*i)->getType().c_str(), (*i)->getName().c_str());
	}
	
	if (*selectedObj) {
		printf("selected item: %s %s\n",
			(*selectedObj)->getType().c_str(),
			(*selectedObj)->getName().c_str());
	}

	printf("active mode %d\n", objectMode);
}

void SceneEditor::recieveMessage(sMessage message) 
{
	//printf("Editor recieving message %s with %f\n", message.name.c_str(), message.fValue);
	if (selectedObj == objects.end()) {
		return;
	}

	if (message.name == "number") {
		// dsp controls
		sMessage itemMessage;
		itemMessage.name = "dsp";
		itemMessage.iValue = message.iValue;
		(*selectedObj)->recieveMessage(itemMessage);
	}
	if (message.name == "simple press") {
		if (message.sValue == "f1") {
			printInfo();
		}

		if (message.sValue == "down") {
			nextObject();
		}

		if (message.sValue == "up") {
			previousObject();
		}

		// Individual obj mode
		if (message.sValue == "t") {
			objectMode = TRANS;
		}

		if (message.sValue == "r") {
			objectMode = ROT;
		}

		if (message.sValue == "z") {
			objectMode = SCALE;
		}

		if (message.sValue == "v") {
			objectMode = ANGLES;
		}

		if (message.sValue == "space") {
			sMessage itemMessage;
			itemMessage.name = "toggle";
			(*selectedObj)->recieveMessage(itemMessage);
			return;
		}

		// General edition mode
		if (message.sValue == "o") {
			setEditMode(OBJS);
		}
	}
	else if (message.name == "press with shift") {
		sMessage itemMessage;

		switch (objectMode) {
		case TRANS:
			itemMessage.name = "translate";

			if (message.sValue == "a")
			{
				itemMessage.v3Value = glm::vec3(-1.0f, 0.0f, 0.0f);
				(*selectedObj)->recieveMessage(itemMessage);
				changeObject();
				return;
			}
			if (message.sValue == "d")
			{
				itemMessage.v3Value = glm::vec3(1.0f, 0.0f, 0.0f);
				(*selectedObj)->recieveMessage(itemMessage);
				changeObject();
				return;
			}
			if (message.sValue == "q")
			{
				itemMessage.v3Value = glm::vec3(0.0f, -0.0f, -1.0f);
				(*selectedObj)->recieveMessage(itemMessage);
				changeObject();
				return;
			}
			if (message.sValue == "e")
			{
				itemMessage.v3Value = glm::vec3(0.0f, 0.0f, 1.0f);
				(*selectedObj)->recieveMessage(itemMessage);
				changeObject();
				return;
			}
			if (message.sValue == "w")
			{
				itemMessage.v3Value = glm::vec3(0.0f, 1.0f, 0.0f);
				(*selectedObj)->recieveMessage(itemMessage);
				changeObject();
				return;
			}
			if (message.sValue == "s")
			{
				itemMessage.v3Value = glm::vec3(0.0f, -1.0f, -0.0f);
				(*selectedObj)->recieveMessage(itemMessage);
				changeObject();
				return;
			}
			break;
		case ROT:
			itemMessage.name = "rotate";

			if (message.sValue == "a")
			{
				(*selectedObj)->yaw(-1.0);
				return;
			}
			if (message.sValue == "d")
			{
				(*selectedObj)->yaw(1.0f);
				return;
			}
			if (message.sValue == "q")
			{
				(*selectedObj)->barrelRoll(-1.0f);
				return;
			}
			if (message.sValue == "e")
			{
				(*selectedObj)->barrelRoll(1.0f);
				return;
			}
			if (message.sValue == "w")
			{
				(*selectedObj)->pitch(1.0f);
				return;
			}
			if (message.sValue == "s")
			{
				(*selectedObj)->pitch(-1.0f);
				return;
			}
			break;
		case SCALE:
			itemMessage.name = "scale";

			if (message.sValue == "w")
			{
				itemMessage.fValue = 1.0f;
				(*selectedObj)->recieveMessage(itemMessage);
				return;
			}
			if (message.sValue == "s")
			{
				itemMessage.fValue = -1.0f;
				(*selectedObj)->recieveMessage(itemMessage);
				return;
			}
			if (message.sValue == "a")
			{
				itemMessage.fValue = 1.0f;
				itemMessage.name = "quadAtten";
				(*selectedObj)->recieveMessage(itemMessage);
				return;
			}
			if (message.sValue == "d")
			{
				itemMessage.fValue = -1.0f;
				itemMessage.name = "quadAtten";
				(*selectedObj)->recieveMessage(itemMessage);
				return;
			}
			break;
		case ANGLES:
			if (message.sValue == "w")
			{
				itemMessage.name = "innerAngle";
				itemMessage.fValue = 1.0f;
				(*selectedObj)->recieveMessage(itemMessage);
				return;
			}
			if (message.sValue == "s")
			{
				itemMessage.name = "innerAngle";
				itemMessage.fValue = -1.0f;
				(*selectedObj)->recieveMessage(itemMessage);
				return;
			}
			if (message.sValue == "a")
			{
				itemMessage.name = "outerAngle";
				itemMessage.fValue = -1.0f;
				(*selectedObj)->recieveMessage(itemMessage);
				return;
			}
			if (message.sValue == "d")
			{
				itemMessage.name = "outerAngle";
				itemMessage.fValue = 1.0f;
				(*selectedObj)->recieveMessage(itemMessage);
				return;
			}
			break;
		}

		if (message.sValue == "up") {
			itemMessage.name = "volume";
			itemMessage.fValue = 1.0f;
			(*selectedObj)->recieveMessage(itemMessage);
			return;
		}

		if (message.sValue == "down") {
			itemMessage.name = "volume";
			itemMessage.fValue = -1.0f;
			(*selectedObj)->recieveMessage(itemMessage);
			return;
		}

		if (message.sValue == "left") {
			itemMessage.name = "pitch";
			itemMessage.fValue = -1.0f;
			(*selectedObj)->recieveMessage(itemMessage);
			return;
		}

		if (message.sValue == "right") {
			itemMessage.name = "pitch";
			itemMessage.fValue = 1.0f;
			(*selectedObj)->recieveMessage(itemMessage);
			return;
		}
	}
	else {
		printf("Unrecognized Message by SceneEditor: %s\n", message.name.c_str());
	}
}