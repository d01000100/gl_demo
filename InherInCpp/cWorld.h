#pragma once
#include "iShip.h"
#include <vector>


class cWorld
{
public:
//	cWorld();		// Now private
	void AddShipToWorld( iShip* pTheShip );

	static cWorld* CreateAWorld(void);
	
	// or automatic thread safe...
//	static cWorld* GetWorld(void);

	static int x;
private:

	// remember what ships we made
	std::vector< iShip* > m_vec_pShips;

	// Move constructor into private
	cWorld();

	// static variable means there's only one of them
	// shared will ALL the classes. 
	static cWorld* m_pTheOneAndOnlyWorldObject;
};