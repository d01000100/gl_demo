#ifndef _PhysicsStuff_HG_
#define _PhysicsStuff_HG_

#include <vector>
#include "cGameObject.h"

void PhysicsUpdate(std::vector<cGameObject>& vecGameObjects,
				   float deltaTime);

#endif
