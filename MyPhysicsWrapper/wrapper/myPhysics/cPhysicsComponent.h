#pragma once
#include "physics/interfaces/iPhysicsComponent.h"
#include "phys/cRigidBody.h"

namespace nPhysics
{
	class cPhysicsComponent : public iPhysicsComponent
	{
	public:
		~cPhysicsComponent();
	protected:
		cPhysicsComponent(phys::sRigidBodyDef bodyDef, phys::iShape* pShape);
	private:
		phys::cRigidBody mRigidBody;
	};
}
