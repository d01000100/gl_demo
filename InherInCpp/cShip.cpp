#include "cShip.h"

#include <iostream>

cShip::cShip()
{
	std::cout << "a CShip is created" << std::endl;
}
cShip::~cShip()
{
	std::cout << "a CShip is destroyed" << std::endl;
}


void cShip::Shoot(iShipDamageInterface* pShipToShoot)
{
	std::cout << "cShip: " << this->pilotName << " Shoots " << std::endl;
	// 
	pShipToShoot->TakeDamage(10.0f);

	//dynamic_cast<iShip*>(pShipToShoot)->Fly();

	return;
}

void cShip::Fly(void)
{
	std::cout<< "cShip: " <<  this->pilotName << " Flies" << std::endl;
	return;
}

void cShip::Land(void)
{
	std::cout << "cShip lands" << std::endl;
}

void cShip::TakeDamage(float amount)
{
	std::cout << "cShip " << this->pilotName << 
		" takes " << amount << " of damage!" << std::endl;

	this->health -= amount;
}