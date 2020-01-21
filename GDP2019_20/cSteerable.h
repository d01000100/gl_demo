#pragma once
#include "cGameObject.h"
class cSteerable
{
public:
	float maxVel;
	cGameObject* agent;
	cSteerable(cGameObject *a);
	void Seek(cGameObject* target, float deltaTime);
};

