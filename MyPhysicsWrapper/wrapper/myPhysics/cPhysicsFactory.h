#pragma once
#include "physics/interfaces/iPhysicsFactory.h"

namespace nPhysics
{
	// Class that instanciates different objects involved
	// in the physics simulation of "MyPhysicsLibrary".
	// This way, the client doesn't need to know about the
	// wrapper itself.
	class cPhysicsFactory : iPhysicsFactory
	{
	public:
		// No properties to initialize
		cPhysicsFactory() = default;
		// No properties to clean up
		virtual ~cPhysicsFactory() = default;
		virtual iPhysicsWorld* CreateWorld() override;
		virtual iBallComponent* CreateBall(const sBallDef& def) override;
		virtual iPlaneComponent* CreatePlane(const sPlaneDef& def) override;
	private:
		// Not desired constructors
		cPhysicsFactory(const cPhysicsFactory& other) = delete;
		cPhysicsFactory& operator=(const cPhysicsFactory& other) = delete;
	};
}

