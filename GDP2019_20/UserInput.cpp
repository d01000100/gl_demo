
#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <fstream>

#include "GFLW_callbacks.h"
#include "globalStuff.h"			// for find object
#include "Camera.h"
#include "Scene.h"
#include "SceneEditor.h"
#include "JSON_IO.h"

#include <stdio.h>		// for fprintf()

bool isShiftKeyDownByAlone(int mods);
bool isCtrlKeyDownByAlone(int mods);

const float CAMERAROTATIONSPEED = 0.05f; // glm::radians(0.1f);
const float CAMERAZOOMSPEED = 2.0f;

const float TRANSLATION_STEP = 0.5f;
const float ROTATION_STEP = glm::radians(0.5f);

void camera_around_target(int key, int scancode, int action, int mods) {
	Camera* theCamera = Camera::getTheCamera();

	if (!isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods))
	{
		if (key == GLFW_KEY_A)
		{
			theCamera->moveLeft(CAMERAROTATIONSPEED);
		}
		if (key == GLFW_KEY_D)
		{
			theCamera->moveRight(CAMERAROTATIONSPEED);
		}
		if (key == GLFW_KEY_Q)
		{
			theCamera->zoom(CAMERAZOOMSPEED);
		}
		if (key == GLFW_KEY_E)
		{
			theCamera->zoom(-CAMERAZOOMSPEED);
		}
		if (key == GLFW_KEY_W)
		{
			theCamera->moveUp(CAMERAROTATIONSPEED);
		}
		if (key == GLFW_KEY_S)
		{
			theCamera->moveDown(CAMERAROTATIONSPEED);
		}
	}
}

void editScene(int key, int scancode, int action, int mods) 
{
	SceneEditor* theEditor = SceneEditor::getTheEditor();
	Scene* theScene = Scene::getTheScene();

	if (!isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods))
	{
		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
			switch (theEditor->getEditMode()) {
			case OBJS:
				theEditor->nextObject();
				break;
			case LIGHTS:
				theEditor->nextLight();
				break;
			}
		}

		if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			switch (theEditor->getEditMode()) {
			case OBJS:
				theEditor->previousObject();
				break;
			case LIGHTS:
				theEditor->previousLight();
				break;
			}
		}
		// General edition mode
		if (key == GLFW_KEY_O && action == GLFW_PRESS) {
			theEditor->setEditMode(OBJS);
		}
		if (key == GLFW_KEY_L && action == GLFW_PRESS) {
			theEditor->setEditMode(LIGHTS);
		}

		if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
			saveScene(theScene, ::scene_filename);
		}

		if (key == GLFW_KEY_F5 && action == GLFW_PRESS) {
			theScene->reloadScene(::scene_filename);
		}

		if (key == GLFW_KEY_PAUSE && action == GLFW_PRESS) {
			::isPlaying = !::isPlaying;
		}
	}

}

void editObject(int key, int scancode, int action, int mods)
{
	SceneEditor* theEditor = SceneEditor::getTheEditor();

	if (!isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods))
	{
		// Individual obj mode
		if (key == GLFW_KEY_T && action == GLFW_PRESS) {
			theEditor->setObjectMode(TRANS);
		}

		if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			theEditor->setObjectMode(ROT);
		}

		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
			theEditor->setObjectMode(SCALE);
		}

		if (key == GLFW_KEY_V && action == GLFW_PRESS) {
			theEditor->setObjectMode(ANGLES);
		}

		if (key == GLFW_KEY_SPACE &&
			action == GLFW_PRESS) {
			if (theEditor->getEditMode() == LIGHTS) {
				theEditor->toggleLight();
			}
			if (theEditor->getEditMode() == OBJS) {
				theEditor->toggleWireframe();
			}
		}
	}

	// Edit mode
	if (isShiftKeyDownByAlone(mods))
	{
		switch (theEditor->objectMode) {
		case TRANS: {
			glm::vec3 direction = glm::vec3(0.0f);
			if (key == GLFW_KEY_A)
			{
				direction = glm::vec3(-1.0f, 0.0f, 0.0f);
			}
			if (key == GLFW_KEY_D)
			{
				direction = glm::vec3(1.0f, 0.0f, 0.0f);
			}
			if (key == GLFW_KEY_Q)
			{
				direction = glm::vec3(0.0f, 0.0f, -1.0f);
			}
			if (key == GLFW_KEY_E)
			{
				direction = glm::vec3(0.0f, 0.0f, 1.0f);
			}
			if (key == GLFW_KEY_W)
			{
				direction = glm::vec3(0.0f, 1.0f, 0.0f);
			}
			if (key == GLFW_KEY_S)
			{
				direction = glm::vec3(0.0f, -1.0f, 0.0f);
			}
			theEditor->translateObject(direction * TRANSLATION_STEP);
			break;
		}
		case ROT: {
			glm::vec3 rotation = glm::vec3(0.0f);
			if (key == GLFW_KEY_A)
			{
				rotation = glm::vec3(0.0f, 1.0f, 0.0f);
			}
			if (key == GLFW_KEY_D)
			{
				rotation = glm::vec3(0.0f, -1.0f, 0.0f);
			}
			if (key == GLFW_KEY_Q)
			{
				rotation = glm::vec3(0.0f, 0.0f, -1.0f);
			}
			if (key == GLFW_KEY_E)
			{
				rotation = glm::vec3(0.0f, 0.0f, 1.0f);
			}
			if (key == GLFW_KEY_W)
			{
				rotation = glm::vec3(-1.0f, 0.0f, 0.0f);
			}
			if (key == GLFW_KEY_S)
			{
				rotation = glm::vec3(1.0f, 0.0f, 0.0f);
			}
			theEditor->rotateObject(rotation * ROTATION_STEP);
			break;
		}
		case SCALE: {
			if (action != GLFW_RELEASE) {
				if (key == GLFW_KEY_W)
				{
					theEditor->scaleObject(1.1f);
				}
				if (key == GLFW_KEY_S)
				{
					theEditor->scaleObject(0.9f);
				}
				if (key == GLFW_KEY_A && theEditor->getEditMode() == LIGHTS)
				{
					theEditor->changeQuadAtten(1.1f);
				}
				if (key == GLFW_KEY_D && theEditor->getEditMode() == LIGHTS)
				{
					theEditor->changeQuadAtten(0.9f);
				}
			}
			break;
		}
		case ANGLES:
			if (theEditor->getEditMode() == LIGHTS) {
				if (key == GLFW_KEY_W)
				{
					theEditor->changeOuterAngle(1.02f);
				}
				if (key == GLFW_KEY_S)
				{
					theEditor->changeOuterAngle(0.98f);
				}
				if (key == GLFW_KEY_A)
				{
					theEditor->changeInnerAngle(0.98f);
				}
				if (key == GLFW_KEY_D)
				{
					theEditor->changeInnerAngle(1.02f);
				}
				break;
			}
		}
	}
}

void navigation_camera(int key, int scancode, int action, int mods) {
	Camera* theCamera = Camera::getTheCamera();

	if (!isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods))
	{
		if (key == GLFW_KEY_A)
		{
			theCamera->turnHorizontally(CAMERAROTATIONSPEED);
		}
		if (key == GLFW_KEY_D)
		{
			theCamera->turnHorizontally(-CAMERAROTATIONSPEED);
		}
		if (key == GLFW_KEY_Q)
		{
			theCamera->walk(-CAMERAZOOMSPEED);
		}
		if (key == GLFW_KEY_E)
		{
			theCamera->walk(CAMERAZOOMSPEED);
		}
		if (key == GLFW_KEY_W)
		{
			theCamera->turnVertically(-CAMERAROTATIONSPEED);
		}
		if (key == GLFW_KEY_S)
		{
			theCamera->turnVertically(CAMERAROTATIONSPEED);
		}
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Camera* theCamera = Camera::getTheCamera();
	Scene* theScene = Scene::getTheScene();
	SceneEditor* theEditor = SceneEditor::getTheEditor();

	camera_around_target(key, scancode, action, mods);
	//navigation_camera(key, scancode, action, mods);
	editScene(key, scancode, action, mods);
	editObject(key, scancode, action, mods);

	// Moving the pirate ship in a certain direction
	if (isCtrlKeyDownByAlone(mods))
	{
		
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

bool isShiftKeyDownByAlone(int mods)
{
	return mods == GLFW_MOD_SHIFT;
}

bool isCtrlKeyDownByAlone(int mods)
{
	return mods == GLFW_MOD_CONTROL;
}