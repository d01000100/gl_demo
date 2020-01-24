#include "Gameplay.h"

void Gameplay::init(GLFWwindow* w)
{
	theScene = Scene::getTheScene();
	playerBullet = NULL;
	player = (cGameObject*)theScene->findItem("player");
	if (!player) { printf("No player in gameplay!!\n"); }
	this->window = w;
	shootTimer = 5.0f;
}

void Gameplay::velocityControls()
{
	float playerSpeed = 20.0f;
	float pitchSpeed = 2.0f;
	if (player && player->physics)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			player->physics->velocity = player->getDirection() * playerSpeed;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			player->physics->velocity = player->getDirection() * -playerSpeed;
		if (glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS)
			player->physics->velocity = glm::vec3(0);

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			player->yaw(pitchSpeed);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			player->yaw(-pitchSpeed);
	}
}

void Gameplay::update(float deltaTime)
{
	velocityControls();
	Shoot();

	if (shootTimer < 4.0f) {
		shootTimer += deltaTime;
	}
}

void Gameplay::Shoot()
{
	if (shootTimer > 4.0f &&
		glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		playerBullet = new cGameObject(player);
		playerBullet->meshName = "sphere_model";
		playerBullet->scale = 0.5f;
		glm::vec3 velocity = player->getDirection() * 50.0f;
		playerBullet->physics->velocity = velocity;
		playerBullet->tags.clear();
		playerBullet->tags.insert("player");
		playerBullet->tags.insert("bullet");
		playerBullet->textures.clear();
		playerBullet->diffuseColor = Colors::white;
		playerBullet->lifeTime = 4.0f;
		theScene->addItem(playerBullet);
		shootTimer = 0;
	}
}
