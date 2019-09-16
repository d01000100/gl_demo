#include "cFastShip.h"
#include <iostream>

cFastShip::cFastShip()
{
	std::cout << "A cFastShip is created" << std::endl;
	this->pilotName = "Robin";
	this->colour = "Yellow";

}
cFastShip::~cFastShip()
{
	std::cout << "a cFastShip is destroyed" << std::endl;
}

void cFastShip::Fly(void)
{
	std::cout << "cFastShip: " << this->pilotName << " Flies" << std::endl;
	std::cout << "cFastShip: " << this->colour << std::endl;
	
	std::cout << "But it flies really fast";

	return;
}

void cFastShip::TakeDamage(float amount)
{
	std::cout << "cFastShip " << this->pilotName <<
		" takes " << amount << " of damage!" << std::endl;

	this->health -= amount;
}

void cFastShip::Shoot(iShipDamageInterface* pShipToShoot)
{
	std::cout << "cFastShip: " << this->pilotName << " Shoots " << std::endl;
	// 
	pShipToShoot->TakeDamage(10.0f);
	return;
}

void cFastShip::Land(void)
{
	std::cout << "cFastShip lands" << std::endl;
}
