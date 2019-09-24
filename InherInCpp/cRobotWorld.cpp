#include "cRobotWorld.h"

void cRobotWorld::AddRobotToWorld(cRobot* pTheRobotToAdd)
{
	this->m_vec_pRobots.push_back(pTheRobotToAdd);
}


void cRobotWorld::AddGarbage(void)
{
	// Pick a random location for the garbage

	sFood newGarbage;
	newGarbage.type = ALUMINUM;
	newGarbage.amount = 1.0f;
	newGarbage.x = 50.0f;
	newGarbage.y = 100.0f;

	this->vecFood.push_back(newGarbage);


}
