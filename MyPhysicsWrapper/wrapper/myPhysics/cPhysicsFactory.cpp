#include "cPhysicsFactory.h"
#include "includes.h"

nPhysics::iPhysicsWorld* nPhysics::cPhysicsFactory::CreateWorld()
{
	return new cPhysicsWorld();
}

nPhysics::iBallComponent* nPhysics::cPhysicsFactory::CreateBall(const sBallDef& def)
{
	return new cBallComponent(def);
}

nPhysics::iPlaneComponent* nPhysics::cPhysicsFactory::CreatePlane(const sPlaneDef& def)
{
	return new cPlaneComponent(def);
}
