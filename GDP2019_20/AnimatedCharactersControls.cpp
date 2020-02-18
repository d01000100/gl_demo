#include "AnimatedCharactersControls.h"
#include "globalStuff.h"

std::vector<cGameObject*> AnimatedCharactersControls::characters;
int AnimatedCharactersControls::selectedCharacter = 0;
std::set<int> AnimatedCharactersControls::pressedKeys;

cGameObject* AnimatedCharactersControls::selectedGameObject()
{
	return characters[selectedCharacter];
}

AnimationManager* AnimatedCharactersControls::selectedAnimManager()
{
	return selectedGameObject()->animManager;
}

void AnimatedCharactersControls::next()
{
	selectedAnimManager()->changeAnimation("idle");
	selectedCharacter++;
	if (selectedCharacter >= characters.size())
	{
		selectedCharacter = 0;
	}
}

void AnimatedCharactersControls::action()
{
	selectedAnimManager()->changeAnimation("action");
}

void AnimatedCharactersControls::updateKeyState(int key)
{
	int keyState = glfwGetKey(::window, key);
	if (keyState == GLFW_PRESS)
	{
		pressedKeys.insert(key);
	}
	if (keyState == GLFW_RELEASE)
	{
		pressedKeys.erase(key);
	}
}

/*
 * Is `key` pressed on this frame and not on the frame before??
 */
bool AnimatedCharactersControls::gotPressed(int key)
{
	return pressedKeys.count(key) == 0 &&
		glfwGetKey(::window, key) == GLFW_PRESS;
}

/*
 * Is `key` been released on this frame and not on the frame before??
 */
bool AnimatedCharactersControls::gotReleased(int key)
{
	return pressedKeys.count(key) &&
		glfwGetKey(::window, key) == GLFW_RELEASE;
}

void AnimatedCharactersControls::inputListener()
{
	if (glfwGetKey(::window, GLFW_KEY_I) == GLFW_PRESS)
	{
		selectedAnimManager()->changeAnimation("walk");
	}
	
	if (gotReleased(GLFW_KEY_I))
	{
		selectedAnimManager()->changeAnimation("idle");
	}
	
	if (gotPressed(GLFW_KEY_RIGHT_CONTROL) ||
		gotPressed(GLFW_KEY_LEFT_CONTROL))
	{
		selectedAnimManager()->changeAnimation("action");
	}
	/*
	 * We are interested that these keys ONLY do stuff
	 * when we PRESS them. Not while they're pressed
	 */
	updateKeyState(GLFW_KEY_LEFT_CONTROL);
	updateKeyState(GLFW_KEY_RIGHT_CONTROL);
	updateKeyState(GLFW_KEY_SPACE);
	updateKeyState(GLFW_KEY_I);
}
