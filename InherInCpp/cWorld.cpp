#include "cWorld.h"
#include <iostream>
#include <Windows.h>

//static 
int cWorld::x = 0;

//static 
cWorld* cWorld::m_pTheOneAndOnlyWorldObject = NULL;

//static cWorld theOneAndOnlyWorld;

////static
//cWorld* cWorld::GetWorld(void)
//{
//	return &theOneAndOnlyWorld;
//}


//static 
cWorld* cWorld::CreateAWorld(void)
{
//	EnterCriticalSection();

	if (cWorld::m_pTheOneAndOnlyWorldObject == NULL)
	{
		cWorld::m_pTheOneAndOnlyWorldObject = new cWorld();
	}

//	LeaveCriticalSection();

	return cWorld::m_pTheOneAndOnlyWorldObject;
}

cWorld::cWorld()
{
	std::cout << "A cWorld has been created" << std::endl;
	return;
}


void cWorld::AddShipToWorld(iShip* pTheShip)
{
	this->m_vec_pShips.push_back(pTheShip);

	return;
}