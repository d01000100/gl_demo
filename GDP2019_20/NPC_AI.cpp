#include "NPC_AI.h"
#include "Scene.h"
#include <glm/glm.hpp>

NPC_AI::NPC_AI()
{
	Scene* theScene = Scene::getTheScene();
	// "plantilla" is template in spanish
	cGameObject* plantilla = (cGameObject*)theScene->findItem("ship_template");
	if (plantilla)
	{
		cGameObject* enemyGO = new cGameObject(plantilla);
		enemyGO->position += glm::vec3(30, 0, -30);
		enemyGO->isVisible = true;
		enemyGO->tags.insert("enemy");
		cSteerable *pSteerable = new cSteerable(enemyGO);
		sEnemy *enemy = new sEnemy(pSteerable);
		enemy->type = "pursue";
		enemy->mode = "approach";
		enemies.push_back(enemy);
		theScene->addItem(enemyGO);
	}
	else {
		printf("Didn't found the ship template\n");
	}
}

void NPC_AI::Update(float deltaTime)
{
	std::vector<sEnemy*>::iterator itEnemies;
	Scene* theScene = Scene::getTheScene();
	cGameObject* player = (cGameObject*)theScene->findItem("player");
	if (!player) { printf("Player not found\n"); return; }
	for (itEnemies = enemies.begin(); itEnemies != enemies.end(); itEnemies++)
	{
		sEnemy* enemy = *itEnemies;
		//UpdateBooEnemy(enemy, deltaTime);
		//UpdateDodger(enemy, deltaTime);
		//UpdateWanderer(enemy, deltaTime);
		UpdateShooter(enemy, deltaTime);
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
	glm::vec3 playerToEnemy = enemyGO->position - player->position;
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
	}
	enemySteer->Pursue(player, deltaTime);
}

void NPC_AI::UpdateWanderer(sEnemy* wanderer, float deltaTime)
{
	
	if (wanderer->mode == "wander")
	{
		wanderer->pSteerable->Wander(deltaTime);
		if (wanderer->timer > 6.0f)
		{
			wanderer->timer = 0.0f;
			wanderer->mode = "idle";
			wanderer->pSteerable->gameObject->physics->velocity = glm::vec3(0);
		}
	}
	else if (wanderer->mode == "idle")
	{
		if (wanderer->timer > 3.0f)
		{
			wanderer->timer = 0.0f;
			wanderer->mode = "wander";
		}
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
		float shootDistance = (shooterSteer->stopDistance + shooterSteer->approachDistance) / 2;
		if (glm::distance(shooterGO->position, player->position) < shootDistance)
		{
			cGameObject* plantilla = (cGameObject*)theScene->findItem("bullet_template");
			if (plantilla)
			{
				cGameObject* bullet = new cGameObject(plantilla);
				bullet->position = shooterGO->position;
				glm::vec3 bulletDir = glm::normalize(player->position - shooterGO->position);
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
}