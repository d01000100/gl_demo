#pragma once
#include "physics/interfaces/iPhysicsWorld.h"
#include "phys/phys.h"

namespace  nPhysics
{
	// A collection of "iPhysicsComponents" that manages
	// their movement and collision simulation.
	// Delegates to a "phys::cWorld"
	class cPhysicsWorld : public iPhysicsWorld
	{
	public:
		// Calls the default constructor of "mWorld"
		cPhysicsWorld() = default;
		// Calls the default destructor of "mWorld"
		virtual ~cPhysicsWorld() = default;
		// Runs a "frame" of simulation on the contained components
		// dt : the lenght of the timestep to simulate
		virtual void Update(float dt) override;
		// Adds the cRigidBody of the iPhysicsComponent to the cWorld
		// Returns true if:
		//    There was an addition to the world.
		//    This particular rigid body was added to the world.
		//    (It must not have already been contained in the world)
		// Returns false if:
		//    There was no addition to the world.
		//    A null pointer is passed in.
		//    This particular rigid body is already in the world, hense not added again.
		virtual bool AddComponent(iPhysicsComponent* component) override;
		// Removes the cRigidBody of the iPhysicsComponent to the cWorld
		// Returns true if:
		//    There was a removal from the world.
		//    This particular rigid body existed in the world and was removed.
		// Returns false if:
		//    There was no removal from the world.
		//    A null pointer was passed in.
		//    This particular rigid body was not in the world, hense not removed.
		virtual bool RemoveComponent(iPhysicsComponent* component) override;
	private:
		// Concrete physics world implementation.
		// From "MyPhysicsLibrary"
		phys::cWorld mWorld;
		// Not desired constructors
		cPhysicsWorld(const cPhysicsWorld& other) = delete;
		cPhysicsWorld& operator=(const cPhysicsWorld& other) = delete;
	};
}

