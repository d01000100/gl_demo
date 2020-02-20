#include "AnimatedCharactersControls.h"
#include "globalStuff.h"
#include "Camera.h"

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
	selectedAnimManager()->changeAnimation("idle", 1.0);
	selectedGameObject()->diffuseColor = Colors::grey;
	selectedGameObject()->physics->velocity = glm::vec3(0);
	
	selectedCharacter++;
	if (selectedCharacter >= characters.size())
	{
		selectedCharacter = 0;
	}
	selectedGameObject()->diffuseColor = Colors::red;

	Camera* theCamera = Camera::getTheCamera();
	theCamera->setTarget(selectedGameObject()->getPos());
}

void AnimatedCharactersControls::action()
{
	selectedAnimManager()->changeAnimation("action", 1.0);
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

void AnimatedCharactersControls::tankControls()
{
	float walkVelocity = 70.0f, runSpeed = 120.0f;
	if (glfwGetKey(::window, GLFW_KEY_I) == GLFW_PRESS)
	{
		glm::vec3 direction = selectedGameObject()->getDirection();
		if (glfwGetKey(::window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(::window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
		{
			selectedGameObject()->physics->velocity = direction * walkVelocity;
		}
		else
		{
			selectedGameObject()->physics->velocity = direction * runSpeed;
		}
	}
	else if (glfwGetKey(::window, GLFW_KEY_K) == GLFW_PRESS)
	{
		glm::vec3 direction = selectedGameObject()->getDirection();
		if (glfwGetKey(::window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(::window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
		{
			selectedGameObject()->physics->velocity = direction * -walkVelocity;
		} else
		{
			selectedGameObject()->physics->velocity = direction * -runSpeed;
		}
		
	}
	else
	{
		selectedGameObject()->physics->velocity = glm::vec3(0);
	}

	float rotSpeed = 1.f;
	if (glfwGetKey(::window, GLFW_KEY_J) == GLFW_PRESS)
	{
		selectedGameObject()->yaw(rotSpeed);
	}
	if (glfwGetKey(::window, GLFW_KEY_L) == GLFW_PRESS)
	{
		selectedGameObject()->yaw(-rotSpeed);
	}

	Camera* theCamera = Camera::getTheCamera();
	theCamera->setTarget(selectedGameObject()->getPos());
}

void AnimatedCharactersControls::inputListener()
{
	if(gotPressed(GLFW_KEY_TAB))
	{
		next();
	}
	
	if (glfwGetKey(::window, GLFW_KEY_I) == GLFW_PRESS)
	{
		if (glfwGetKey(::window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(::window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
		{
			selectedAnimManager()->changeAnimation("walk", 1.0);
		}
		else
		{
			selectedAnimManager()->changeAnimation("crawl", 1.0);
		}
	}
	else if (glfwGetKey(::window, GLFW_KEY_K) == GLFW_PRESS)
	{
		if (glfwGetKey(::window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(::window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
		{
			selectedAnimManager()->changeAnimation("walk", -1.0);
		} else
		{
			selectedAnimManager()->changeAnimation("crawl", -1.0);
		}
	}
	
	if (gotReleased(GLFW_KEY_I) || 
		gotReleased(GLFW_KEY_K))
	{
		selectedAnimManager()->changeAnimation("idle", 1.0);
	}
	
	if (gotPressed(GLFW_KEY_RIGHT_CONTROL) ||
		gotPressed(GLFW_KEY_LEFT_CONTROL))
	{
		selectedAnimManager()->changeAnimation("action", 1.0);
	}

	if (gotPressed(GLFW_KEY_SPACE))
	{
		selectedAnimManager()->changeAnimation("jump", 1.0);
	}

	tankControls();
	/*
	 * We are interested that these keys ONLY do stuff
	 * when we PRESS them. Not while they're pressed
	 */
	updateKeyState(GLFW_KEY_LEFT_CONTROL);
	updateKeyState(GLFW_KEY_RIGHT_CONTROL);
	updateKeyState(GLFW_KEY_SPACE);
	updateKeyState(GLFW_KEY_I);
	updateKeyState(GLFW_KEY_K);
	updateKeyState(GLFW_KEY_TAB);
}
