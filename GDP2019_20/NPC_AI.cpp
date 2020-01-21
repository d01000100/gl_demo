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
		enemyGO->position += glm::vec3(-30, 0, 0);
		enemyGO->isVisible = true;
		enemyGO->tags.insert("enemy");
		sEnemy *enemy = new sEnemy(new cSteerable(enemyGO));
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
		cSteerable* enemy = (*itEnemies)->pSteerable;
		cGameObject* enemyGO = enemy->agent;
		enemy->Seek(player, deltaTime);
	}
}
