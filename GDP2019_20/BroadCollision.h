#pragma once
#include <vector>

#include "cPhysics.h"
#include "AABBGrid.h"

class BroadCollision
{
private:
	static cPhysics physicsStuff;
	static float collisionPointSize;
public:
	static std::vector<sCollisionInfo> detectCollisions(AABBGrid* grid, cGameObject* mobile);
	static void collisionsReact(std::vector<sCollisionInfo>, cGameObject* mobile);
};

