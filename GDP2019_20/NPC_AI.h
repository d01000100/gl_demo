#pragma once
#include <vector>
#include "cSteerable.h"

struct sEnemy
{
	cSteerable *pSteerable;
	std::string mode, type;
	cGameObject* bullet;
	float timer;
	sEnemy(cSteerable* s) :
		pSteerable(s),
		timer(0),
		bullet(NULL){}
};

class NPC_AI
{
public:
	std::vector<sEnemy*> enemies;
	NPC_AI();
	void Update(float deltaTime);
private:
	void UpdateBooEnemy(sEnemy* boo, float deltaTime);
	void UpdateDodger(sEnemy* boo, float deltaTime);
	void UpdateWanderer(sEnemy* wanderer, float deltaTime);
	void UpdateShooter(sEnemy* shooter, float deltaTime);
};

