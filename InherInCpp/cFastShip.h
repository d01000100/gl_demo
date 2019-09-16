#pragma once
#include <string>
#include "cShip.h"
#include "iShipDamageInterface.h"


class cFastShip : 
	public iShip,
	public iShipDamageInterface {
public:
	cFastShip();
	virtual~cFastShip();

	virtual void Shoot(iShipDamageInterface* pShipToShoot);
	virtual void Fly(void);
	virtual void Land(void);


	virtual void TakeDamage(float amount);




	std::string colour;
	std::string pilotName;
	float speed;
	float bullets;
	float fuel;
	float health = 100.0f;


};