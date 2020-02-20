#pragma once
#include "cGameObject.h"
#include <set>

struct AnimatedCharactersControls
{
	static std::vector<cGameObject*> characters;
	static int selectedCharacter;
	static std::set<int> pressedKeys;

	static cGameObject* selectedGameObject();
	static AnimationManager* selectedAnimManager();
	static void next();
	static void action();
	static void inputListener();
	static void updateKeyState(int key);
	static bool gotPressed(int key);
	static bool gotReleased(int key);
	static void tankControls();
};
