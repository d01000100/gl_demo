#pragma once
#include <vector>
#include "cSteerable.h"

struct sEnemy
{
	cSteerable *pSteerable;
	std::string mode, type;
	sEnemy(cSteerable* s) :
		pSteerable(s) {}
};

class NPC_AI
{
public:
	std::vector<sEnemy*> enemies;
	NPC_AI();
	void Update(float deltaTime);
};

