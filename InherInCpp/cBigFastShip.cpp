#include "cBigFastShip.h"

#include <iostream>

cBigFastShip::cBigFastShip()
{
	std::cout << "a cBigFastShip is created" << std::endl;
}
cBigFastShip::~cBigFastShip()
{
	std::cout << "a cBigFastShip is destroyed" << std::endl;
}

void cBigFastShip::Shoot(iShipDamageInterface* pShipToShoot)
{
	std::cout << "cBigFastShip:Shoot() BANG!" << std::endl;

}


void cBigFastShip::Fly(void)
{
	std::cout << "cBigFastShip: " << this->pilotName << " Flies" << std::endl;
	return;
}

void cBigFastShip::TakeDamage(float amount)
{
	std::cout << "cBigFastShip " << this->pilotName <<
		" takes " << amount << " of damage!" << std::endl;

	this->health -= amount;
}


void cBigFastShip::Land(void)
{
	std::cout << "cBigFastShip lands" << std::endl;
}
