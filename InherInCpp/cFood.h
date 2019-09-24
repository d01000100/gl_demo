#pragma once

enum eGarbageType
{
	ALUMINUM,
	STEEL,
	PLASTIC,
	ELECTRONICS
};

class cFood
{
	eGarbageType type;
	float amount;
	float x, y;		// Location in the world
};