#pragma once
#include "iShip.h"
#include "iShipDamageInterface.h"
#include "cLASER.h"


#include <string>

class cShip :
	public iShip,
	public iShipDamageInterface
{
public:
	cShip();
	virtual ~cShip();
	//virtual void Shoot(iShip* pShipToShoot);
	virtual void Shoot(iShipDamageInterface* pShipToShoot);
	virtual void Fly(void);				// Only thing that's changing
	virtual void Land(void);

	virtual void TakeDamage(float amount);


	std::string colour;
	std::string pilotName;
	float speed;
	float bullets;
	float fuel;
	float health = 100.0f;

	cLASER myLASER;


};