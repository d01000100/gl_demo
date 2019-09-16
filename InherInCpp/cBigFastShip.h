#pragma once
#include "cFastShip.h"
#include "iShipDamageInterface.h"


class cBigFastShip : 
	public iShip,
	public iShipDamageInterface 
{
public:
	cBigFastShip();
	virtual ~cBigFastShip();

	float GiantFreakingLASER;
	//virtual void Land(void);
	virtual void Fly(void);
	virtual void Shoot(iShipDamageInterface* pShipToShoot);
	virtual void Land();

	virtual void TakeDamage(float amount);
	

	std::string colour;
	std::string pilotName;
	float speed;
	float bullets;
	float fuel;
	float health = 100.0f;


};