#include "Gameplay.h"

void Gameplay::init(GLFWwindow* w)
{
	theScene = Scene::getTheScene();
	playerBullet = NULL;
	player = (cGameObject*)theScene->findItem("player");
	if (!player) { printf("No player in gameplay!!\n"); }
	this->window = w;
	shootTimer = 5.0f;
	fireCooldown = 2.0f;
	npc_AI = new NPC_AI();
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
	Collisions();
	if (shootTimer < fireCooldown) {
		shootTimer += deltaTime;
	}
	npc_AI->Update(deltaTime);
}

void Gameplay::Shoot()
{
	if (shootTimer > fireCooldown &&
		glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cGameObject* plantilla = (cGameObject*)theScene->findItem("bullet_template");
		if (plantilla)
		{
			playerBullet = new cGameObject(plantilla);
			playerBullet->friendlyName = "player_bullet";
			glm::vec3 velocity = player->getDirection() * 30.0f;
			playerBullet->physics->velocity = velocity;
			playerBullet->position = player->position;
			playerBullet->tags.insert("player");
			playerBullet->tags.insert("bullet");
			playerBullet->diffuseColor = Colors::white;
			playerBullet->lifeTime = fireCooldown;
			playerBullet->isVisible = true;
			theScene->addItem(playerBullet);
			shootTimer = 0;
		}
		else
		{
			printf("No bullet template!!!\n");
		}
	}
}

void Gameplay::Collisions()
{
	float playerRadius = 2.0f;
	float enemyRadius = 2.0f;
	float bulletRadius = 0.5f;
	std::vector<sEnemy*>::iterator iterEnemies;
	for (iterEnemies = npc_AI->enemies.begin();
		iterEnemies != npc_AI->enemies.end();)
	{
		sEnemy* enemy = *iterEnemies;
		cGameObject* enemyGO = enemy->pSteerable->gameObject;

		if (glm::distance(enemyGO->position, player->position) < playerRadius + enemyRadius)
		{
			// A player has bumped into an enemy
			player->position = glm::vec3(0);
			player->setDirection(glm::vec3(0, 0, 1));
			player->physics->velocity = glm::vec3(0);
		}

		if (enemy->bullet &&
			glm::distance(player->position, enemy->bullet->position) < playerRadius + bulletRadius)
		{
			// The player bumped into a bullet
			player->position = glm::vec3(0);
			player->setDirection(glm::vec3(0, 0, 1));
			player->physics->velocity = glm::vec3(0);

			theScene->removeItem(enemy->bullet->friendlyName);
			enemy->bullet = NULL;
		}

		if (playerBullet && 
			glm::distance(playerBullet->position, enemyGO->position) < enemyRadius + bulletRadius)
		{
			// We've hit an enemy
			theScene->removeItem(playerBullet->friendlyName);
			theScene->removeItem(enemyGO->friendlyName);
			playerBullet = NULL;
			shootTimer = fireCooldown + 1.0f;
			iterEnemies = npc_AI->enemies.erase(iterEnemies);
		}
		else {
			iterEnemies++;
		}
	}
}
