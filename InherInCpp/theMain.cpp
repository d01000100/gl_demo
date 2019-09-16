#include "cShip.h"
#include "cFastShip.h"
#include "cBigFastShip.h"
#include "cSmallShip.h"
#include <iostream>
#include <vector>

int main()
{
	//	cShip myShip;

	//cShip* pShip = new cShip[20];
	//cFastShip* pFS = new cFastShip[2];
	//cBigFastShip* pBFS = new cBigFastShip[20];

	std::vector<iShip*> myShips;

	iShip* pShipA = new cShip();
	myShips.push_back( pShipA );

	iShip* pShipB = new cFastShip();
	myShips.push_back( pShipB );

	iShip* pShipC = new cBigFastShip();
	myShips.push_back( pShipC );

	iShip* pShipD = new cSmallShip();
	myShips.push_back( pShipD );

	std::cout << std::endl;
	pShipA->Shoot(pShipB);

	//myShips[0]->Fly();
	//myShips[1]->Fly();
	//myShips[2]->Fly();
	//myShips[3]->Fly();




}


//cShip ships[2];			// STACK
