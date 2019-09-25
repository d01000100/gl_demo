#pragma once

#include "commonstuff.h"
#include "cRobot.h"
#include "cBattery.h"
#include <vector>

class cWorld
{
public:
	void GenerateWorld(int numRobots, int numBatteries, float worldSize);

	float getWorldSize(void);
private:
	float m_worldSize;

	std::vector<cBattery*> m_vec_pBatteries;
	std::vector<cRobot*> m_vec_pRobots;
};