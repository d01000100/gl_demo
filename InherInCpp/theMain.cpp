//#include "cShip.h"
//#include "cFastShip.h"
//#include "cBigFastShip.h"
//#include "cSmallShip.h"
//#include "cLASER.h"
//#include "cBigFreakingLASER.h"
#include "cShipFactory.h"

#include <iostream>
#include <vector>
#include <fstream>

int main()
{

	std::vector<iShip*> myShips;

	// Load the info from the file
	std::ifstream shipFile("MyShips.txt");
	if (!shipFile.is_open())
	{
		std::cout << "Didn't find file" << std::endl;
		return -1;
	}

	cShipFactory* pFactory = new cShipFactory();

	std::string tempShipType;
	while (shipFile >> tempShipType)
	{
		if (tempShipType == "EOF") { break; }

		std::cout << "About to creat a " 
			<< tempShipType << " ship..." << std::endl;

		iShip* pTheShip = pFactory->CreateShip(tempShipType);

		//iShip* pTheShip = OLD_SCHOOL_CreateShip(tempShipType);

		myShips.push_back( pTheShip);

		std::cout << std::endl;
	}

	iShip* pPoorShipThatsGonnaGetShot = myShips[5];

	myShips[0]->Shoot(pPoorShipThatsGonnaGetShot);
//	myShips[1]->Shoot(pPoorShipThatsGonnaGetShot);
//	myShips[2]->Shoot(pPoorShipThatsGonnaGetShot);
//	myShips[3]->Shoot(pPoorShipThatsGonnaGetShot);

	pFactory->UpgradeShip(myShips[0], "Ship upgrade to fancy LASER");

	myShips[0]->Shoot(pPoorShipThatsGonnaGetShot);

//	iShip* pShipA = new cShip();
//	myShips.push_back( pShipA );
//
//	iShip* pShipB = new cFastShip();
//	myShips.push_back( pShipB );
//
//	iShip* pShipC = new cBigFastShip();
//	myShips.push_back( pShipC );
//
//	iShip* pShipD = new cSmallShip();
//	myShips.push_back( pShipD );

	//std::cout << std::endl;
	//pShipA->Shoot(pShipB);

	//cShip* pSS = new cShip();
	//pSS->Fly();
	//pSS->myLASER.Fire();

	//cBigFastShip* pBFS = new cBigFastShip();
	//pBFS->myLASER.Fire();
	//pBFS->myBFL.Fire();

	//myShips[0]->Fly();
	//myShips[1]->Fly();
	//myShips[2]->Fly();
	//myShips[3]->Fly();



	//cLASER* pL = new cLASER();
	//cBigFreakingLASER* pBFL = new cBigFreakingLASER();


}


//cShip ships[2];			// STACK
