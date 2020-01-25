#pragma once
#include "cGameObject.h"
class cSteerable
{
public:
	float maxVel, approachDistance, stopDistance,
		wanderDistance, wanderRadius,
		evadePower;
	cGameObject* gameObject;
	cSteerable(cGameObject *a);
	void Seek(cGameObject* target, float deltaTime);
	void Flee(cGameObject* target, float deltaTime);
	void Approach(cGameObject* target, float deltaTime);
	void Wander(float deltaTime);
	void Pursue(cGameObject* target, float deltaTime);
	void Evade(cGameObject* target, float deltaTime);
};

