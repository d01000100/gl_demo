#pragma once
#include "phys/phys.h"
#include "physics/interfaces/iPlaneComponent.h"
#include "cPhysicsWorld.h"

namespace nPhysics
{
	// Physics simulation of an infinite plane.
	// Implements the interface "iPlaneComponent"
	// and constructs a "phys::cPlane" to model the plane.
	class cPlaneComponent : public iPlaneComponent
	{
		friend cPhysicsWorld;
	public:
		// Constructs an infinite plane in world space.
		// "defs" contains a point in the plane and a vector
		// normal to that plane.
		cPlaneComponent(sPlaneDef defs);
		virtual ~cPlaneComponent();
		// Gets the transformation matrix of the sphere
		// containing its position in world space.
		virtual void GetTransform(glm::mat4& transformOut) override;
		// Applies a force to the plane; changes its acceleration
		// taking into account its mass.
		virtual void ApplyForce(glm::vec3 force) override;
		// Constructs a string containing useful info of the sphere:
		// shape, position, velocity.
		virtual std::string ToString() override;
		// Applies an impulse directly to the velocity of the sphere.
		void ApplyImpulse(glm::vec3 impulse) override;
	private:
		// Implementation from "MyPhysicsLibrary" of a rigid body
		phys::cRigidBody *mRigidBody;
		// Concrete sphere shape from "MyPhysicsLibrary"
		phys::cPlane *mShape;
		// Not desired constructors
		cPlaneComponent() = delete;
		cPlaneComponent(const cPlaneComponent& other) = delete;
		cPlaneComponent& operator=(const cPlaneComponent& other) = delete;
	};
}
