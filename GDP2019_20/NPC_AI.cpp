#include "NPC_AI.h"
#include "Scene.h"
#include <glm/glm.hpp>
#include "util.h"
#include <iostream>

NPC_AI::NPC_AI() :
	enemyTimer(6),
	enemyCooldown(5)
{}

void NPC_AI::Update(float deltaTime)
{
	// Spawn enemies at certain intervals
	enemyTimer+= deltaTime;
	if (enemyTimer > enemyCooldown)
	{
		spawnEnemy();
		enemyTimer = 0;
	}
	// Update the steering on existing enemies
	std::vector<sEnemy*>::iterator itEnemies;
	for (itEnemies = enemies.begin(); itEnemies != enemies.end(); itEnemies++)
	{
		sEnemy* enemy = *itEnemies;
		cGameObject* enemyGO = enemy->pSteerable->gameObject;
		if (enemy->type == "boo")
		{
			UpdateBooEnemy(enemy, deltaTime);
		}
		if (enemy->type == "dodger")
		{
			UpdateDodger(enemy, deltaTime);
		}
		if (enemy->type == "wanderer")
		{
			UpdateWanderer(enemy, deltaTime);
		}
		if (enemy->type == "shooter")
		{
			UpdateShooter(enemy, deltaTime);
		}
		enemyGO->setDirection(enemyGO->physics->velocity);
	}
}

void NPC_AI::UpdateBooEnemy(sEnemy* boo, float deltaTime)
{
	Scene* theScene = Scene::getTheScene();
	cGameObject* player = (cGameObject*)theScene->findItem("player");
	if (!player) { printf("Player not found\n"); return; }
	cSteerable* enemySteer = boo->pSteerable;
	cGameObject* enemyGO = boo->pSteerable->gameObject;
	glm::vec3 playerDir = player->getDirection();
	glm::vec3 playerToEnemy = enemyGO->getPos() - player->getPos();
	float dotProduct = glm::dot(glm::normalize(playerDir), glm::normalize(playerToEnemy));
	// The dot product will from 1 to 0 if the player is facing the enemy
	if (dotProduct > 0) {
		enemySteer->Flee(player, deltaTime);
		enemyGO->diffuseColor = Colors::blue;
	}
	// and from 0 to -1 if it's facing away from the enemy
	else {
		enemySteer->Seek(player, deltaTime);
		enemyGO->diffuseColor = Colors::red;
	}
}

void NPC_AI::UpdateDodger(sEnemy* boo, float deltaTime)
{
	Scene* theScene = Scene::getTheScene();
	cGameObject* player = (cGameObject*)theScene->findItem("player");
	if (!player) { printf("Player not found\n"); return; }
	cGameObject* bullet = (cGameObject*)theScene->findItem("player_bullet");
	cSteerable* enemySteer = boo->pSteerable;
	cGameObject* enemyGO = boo->pSteerable->gameObject;
	if (bullet) {
		enemySteer->Evade(bullet, deltaTime);
		enemyGO->diffuseColor = Colors::purple;
	}
	else
	{
		enemyGO->diffuseColor = Colors::white;
	}
	enemySteer->Pursue(player, deltaTime);
}

void NPC_AI::UpdateWanderer(sEnemy* wanderer, float deltaTime)
{
	cGameObject* enemyGO = wanderer->pSteerable->gameObject;
	if (wanderer->mode == "wander")
	{
		wanderer->pSteerable->Wander(deltaTime);
		if (wanderer->timer > 6.0f)
		{
			wanderer->timer = 0.0f;
			wanderer->mode = "idle";
			wanderer->pSteerable->gameObject->physics->velocity = glm::vec3(0);
		}
		enemyGO->diffuseColor = Colors::grey;
	}
	else if (wanderer->mode == "idle")
	{
		if (wanderer->timer > 3.0f)
		{
			wanderer->timer = 0.0f;
			wanderer->mode = "wander";
		}
		enemyGO->diffuseColor = Colors::black;
	}
	wanderer->timer += deltaTime;
}

void NPC_AI::UpdateShooter(sEnemy* shooter, float deltaTime)
{
	Scene* theScene = Scene::getTheScene();
	cGameObject* player = (cGameObject*)theScene->findItem("player");
	if (!player) { printf("Player not found\n"); return; }
	cSteerable* shooterSteer = shooter->pSteerable;
	cGameObject* shooterGO = shooterSteer->gameObject;
	if (shooter->mode == "approach")
	{
		shooterSteer->Approach(player, deltaTime);
		shooterGO->diffuseColor = Colors::green;
		float shootDistance = (shooterSteer->stopDistance + shooterSteer->approachDistance) / 2;
		if (glm::distance(shooterGO->getPos(), player->getPos()) < shootDistance)
		{
			cGameObject* plantilla = (cGameObject*)theScene->findItem("bullet_template");
			if (plantilla)
			{
				cGameObject* bullet = new cGameObject(plantilla);
				bullet->setPos(shooterGO->getPos());
				glm::vec3 bulletDir = glm::normalize(player->getPos() - shooterGO->getPos());
				bullet->physics->velocity = bulletDir * 20.0f;
				bullet->tags.insert("enemy");
				bullet->tags.insert("bullet");
				bullet->diffuseColor = Colors::white;
				bullet->isVisible = true;
				bullet->lifeTime = 5.0f;
				theScene->addItem(bullet);
				shooter->bullet = bullet;
			} else
			{
				printf("No bullet template for enemy!!\n");
			}
			shooter->mode = "idle";
			shooterGO->physics->velocity = glm::vec3(0);
		}
	}
	else
	{
		shooterGO->diffuseColor = Colors::yellow;
	}
}

glm::vec3 NPC_AI::getSpawnLocation()
{
	float spawnDistance = 25.0f;
	double angle = glm::radians(randInRange(-180, 180));
	// calculate circle coordinates
	double x = glm::cos(angle) * spawnDistance,
		z = glm::sin(angle) * spawnDistance;
	return glm::vec3(x, 0, z);
}

void NPC_AI::createBoo()
{
	Scene* theScene = Scene::getTheScene();
	// "plantilla" is template in spanish
	cGameObject* plantilla = (cGameObject*)theScene->findItem("ship_template");
	if (plantilla)
	{
		cGameObject* enemyGO = new cGameObject(plantilla);
		enemyGO->setPos(getSpawnLocation());
		enemyGO->isVisible = true;
		enemyGO->tags.insert("enemy");
		cSteerable *pSteerable = new cSteerable(enemyGO);
		sEnemy *enemy = new sEnemy(pSteerable);
		enemy->type = "boo";
		enemies.push_back(enemy);
		theScene->addItem(enemyGO);
	}
	else {
		printf("Didn't found the ship template\n");
	}
}

void NPC_AI::createDodger()
{
	Scene* theScene = Scene::getTheScene();
	// "plantilla" is template in spanish
	cGameObject* plantilla = (cGameObject*)theScene->findItem("ship_template");
	if (plantilla)
	{
		std::cout << "Creating shooter..." << std::endl;
		cGameObject* enemyGO = new cGameObject(plantilla);
		std::cout << "New gameObject..." << std::endl;
		enemyGO->setPos(getSpawnLocation());
		enemyGO->isVisible = true;
		enemyGO->tags.insert("enemy");
		cSteerable *pSteerable = new cSteerable(enemyGO);
		std::cout << "New Steerable..." << std::endl;
		sEnemy *enemy = new sEnemy(pSteerable);
		std::cout << "New Enemy...." << std::endl;
		enemy->type = "dodger";
		enemies.push_back(enemy);
		std::cout << "Added to enemies (" << enemies.size() << ")" << std::endl;
		theScene->addItem(enemyGO);
		std::cout << "Added to scene!" << std::endl << std::endl;
	}
	else
	{
		printf("Didn't found the ship template\n");
	}
}

void NPC_AI::createWanderer()
{
	Scene* theScene = Scene::getTheScene();
	// "plantilla" is template in spanish
	cGameObject* plantilla = (cGameObject*)theScene->findItem("ship_template");
	if (plantilla)
	{
		cGameObject* enemyGO = new cGameObject(plantilla);
		enemyGO->setPos(getSpawnLocation());
		enemyGO->isVisible = true;
		enemyGO->tags.insert("enemy");
		cSteerable *pSteerable = new cSteerable(enemyGO);
		sEnemy *enemy = new sEnemy(pSteerable);
		enemy->type = "wanderer";
		enemy->mode = "wander";
		enemies.push_back(enemy);
		theScene->addItem(enemyGO);
	}
	else {
		printf("Didn't found the ship template\n");
	}
}

void NPC_AI::createShooter()
{
	Scene* theScene = Scene::getTheScene();
	// "plantilla" is template in spanish
	cGameObject* plantilla = (cGameObject*)theScene->findItem("ship_template");
	if (plantilla)
	{
		std::cout << "Creating shooter..." << std::endl;
		cGameObject* enemyGO = new cGameObject(plantilla);
		std::cout << "New gameObject..." << std::endl;
		enemyGO->setPos(getSpawnLocation());
		enemyGO->isVisible = true;
		enemyGO->tags.insert("enemy");
		cSteerable *pSteerable = new cSteerable(enemyGO);
		std::cout << "New Steerable..." << std::endl;
		sEnemy *enemy = new sEnemy(pSteerable);
		std::cout << "New Enemy...." << std::endl;
		enemy->type = "shooter";
		enemy->mode = "approach";
		enemies.push_back(enemy);
		std::cout << "Added to enemies (" << enemies.size() << ")" << std::endl;
		theScene->addItem(enemyGO);
		std::cout << "Added to scene!" << std::endl << std::endl;
	}
	else {
		printf("Didn't found the ship template\n");
	}
}

void NPC_AI::spawnEnemy()
{
	int enemyType = randInRange(0, 4);
	switch (enemyType)
	{
	case 0:
		createBoo();	
		break;
	case 1:
		createDodger();
		break;
	case 2:
		createShooter();
		break;
	case 3:
		createWanderer();
		break;
	}
}
