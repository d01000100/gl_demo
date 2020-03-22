#include "AnimatedCharactersControls.h"
#include "globalStuff.h"
#include "Camera.h"

std::vector<cGameObject*> AnimatedCharactersControls::characters;
int AnimatedCharactersControls::selectedCharacter = 0;
std::set<int> AnimatedCharactersControls::pressedKeys;

cGameObject* AnimatedCharactersControls::selectedGameObject()
{
	if (!characters.empty())
	{
		return characters[selectedCharacter];
	}
	else
	{
		return nullptr;
	}
}

AnimationManager* AnimatedCharactersControls::selectedAnimManager()
{
	if (selectedGameObject())
	{
		return selectedGameObject()->animManager;
	}
	else
	{
		return nullptr;
	}
}

void AnimatedCharactersControls::next()
{
	if (selectedGameObject())
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
}

void AnimatedCharactersControls::action()
{
	if (selectedGameObject())
	{
		selectedAnimManager()->changeAnimation("action", 1.0);
	}
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
	if (selectedGameObject())
	{
		float walkVelocity = 60.0f, runSpeed = 120.0f;
		if (glfwGetKey(::window, GLFW_KEY_A) == GLFW_PRESS)
		{
			selectedGameObject()->setDirection(glm::vec3(-1, 0, 0));
			if (glfwGetKey(::window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
				glfwGetKey(::window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
			{
				selectedGameObject()->physics->velocity.x = -walkVelocity;
			} else
			{
				selectedGameObject()->physics->velocity.x = -runSpeed;
			}
		} else if (glfwGetKey(::window, GLFW_KEY_D) == GLFW_PRESS)
		{
			selectedGameObject()->setDirection(glm::vec3(1, 0, 0));
			if (glfwGetKey(::window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
				glfwGetKey(::window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
			{
				selectedGameObject()->physics->velocity.x = walkVelocity;
			} else
			{
				selectedGameObject()->physics->velocity.x = runSpeed;
			}

		} else
		{
			selectedGameObject()->physics->velocity.x = 0;
		}

		Camera* theCamera = Camera::getTheCamera();
		//theCamera->setTarget(selectedGameObject()->getPos());
	}
}

void AnimatedCharactersControls::inputListener()
{
	if(gotPressed(GLFW_KEY_TAB))
	{
		next();
	}

	if (selectedGameObject())
	{
		if (glfwGetKey(::window, GLFW_KEY_A) == GLFW_PRESS)
		{
			if (glfwGetKey(::window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
				glfwGetKey(::window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
			{
				selectedAnimManager()->changeAnimation("walk", 1.0);
			} else
			{
				selectedAnimManager()->changeAnimation("crawl", 1.0);
			}
		} else if (glfwGetKey(::window, GLFW_KEY_D) == GLFW_PRESS)
		{
			if (glfwGetKey(::window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
				glfwGetKey(::window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
			{
				selectedAnimManager()->changeAnimation("walk", 1.0);
			} else
			{
				selectedAnimManager()->changeAnimation("crawl", 1.0);
			}
		}

		if (gotReleased(GLFW_KEY_A) ||
			gotReleased(GLFW_KEY_D))
		{
			selectedAnimManager()->changeAnimation("idle", 1.0);
		}

		if (gotPressed(GLFW_KEY_ENTER))
		{
			selectedAnimManager()->changeAnimation("action", 1.0);
		}

		if (gotPressed(GLFW_KEY_SPACE))
		{
			selectedAnimManager()->changeAnimation("jump", 1.0);
		}

		if (glfwGetKey(::window, GLFW_KEY_A) == GLFW_PRESS ||
			glfwGetKey(::window, GLFW_KEY_D) == GLFW_PRESS)
		{
			if (gotPressed(GLFW_KEY_S))
			{
				selectedAnimManager()->changeAnimation("roll", 1.0);
			}
		}

		tankControls();
	}
	/*
	 * We are interested that these keys ONLY do stuff
	 * when we PRESS them. Not while they're pressed
	 */
	updateKeyState(GLFW_KEY_ENTER);
	updateKeyState(GLFW_KEY_LEFT_CONTROL);
	updateKeyState(GLFW_KEY_RIGHT_CONTROL);
	updateKeyState(GLFW_KEY_SPACE);
	updateKeyState(GLFW_KEY_A);
	updateKeyState(GLFW_KEY_D);
	updateKeyState(GLFW_KEY_S);
	updateKeyState(GLFW_KEY_TAB);
}
