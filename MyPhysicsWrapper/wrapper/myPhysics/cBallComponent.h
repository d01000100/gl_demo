#pragma once
#include "physics/interfaces/iBallComponent.h"
#include "phys/phys.h"
#include "cPhysicsWorld.h"

namespace nPhysics
{
	// Physics simulation of a sphere.
	// Implements the interface "iBallComponent"
	// and delegates all the functionality to a "cRigidBody".
	class cBallComponent : public iBallComponent
	{
		friend cPhysicsWorld;
	public:
		// Creates a Sphere with the data in "defs".
		cBallComponent(nPhysics::sBallDef defs);
		virtual ~cBallComponent();
		// Gets the transformation matrix of the sphere
		// containing its position in world space.
		virtual void GetTransform(glm::mat4& transformOut) override;
		// Constructs a string containing useful info of the sphere:
		// shape, position, velocity.
		virtual std::string ToString() override;
		// Applies a force to the sphere; changes its acceleration
		// taking into account its mass.
		virtual void ApplyForce(glm::vec3 force) override;
		// Applies an impulse directly to the velocity of the sphere.
		virtual void ApplyImpulse(glm::vec3 impulse) override;
	private:
		// Implementation from "MyPhysicsLibrary" of a rigid body
		phys::cRigidBody *mRigidBody;
		// Concrete sphere shape from "MyPhysicsLibrary"
		phys::cSphere* mShape;
		// Not desired constructors
		cBallComponent() = delete;
		cBallComponent(const cBallComponent& other) = delete;
		cBallComponent& operator=(const cBallComponent& other) = delete;
	};
}
