#include "NPC_AI.h"
#include "Scene.h"

NPC_AI::NPC_AI()
{
	Scene* theScene = Scene::getTheScene();
	// "plantilla" is template in spanish
	cGameObject* plantilla = (cGameObject*)theScene->findItem("ship_template");
	if (plantilla)
	{
		cGameObject* enemyGO = new cGameObject(plantilla);
		enemyGO->position += glm::vec3(0, 0, 15);
		enemyGO->isVisible = true;
		enemyGO->tags.insert("enemy");
		cSteerable *pSteerable = new cSteerable(enemyGO);
		pSteerable->maxVel = 10.0f;
		pSteerable->wanderDistance = 5.0f;
		pSteerable->wanderRadius = 10.0f;
		sEnemy *enemy = new sEnemy(pSteerable);
		enemy->type = "pursue";
		enemies.push_back(enemy);
		theScene->addItem(enemyGO);

		enemyGO = new cGameObject(plantilla);
		enemyGO->position += glm::vec3(0, 0, -15);
		enemyGO->isVisible = true;
		enemyGO->tags.insert("enemy2");
		enemyGO->diffuseColor = Colors::red;
		pSteerable = new cSteerable(enemyGO);
		pSteerable->maxVel = 10.0f;
		pSteerable->wanderDistance = 5.0f;
		pSteerable->wanderRadius = 10.0f;
		enemy = new sEnemy(pSteerable);
		enemy->type = "seek";
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
		if (enemy->type == "seek")
			enemy->pSteerable->Wander(deltaTime);
		if (enemy->type == "pursue")
			(*itEnemies)->pSteerable->Evade(player, deltaTime);
	}
}
