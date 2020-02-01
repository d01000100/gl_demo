#include "cRigidBody.h"  // My header
#include "game_math.h"

namespace phys
{
	cRigidBody::cRigidBody(const sRigidBodyDef& def, iShape* shape) :
		mShape(shape),
		mPreviousPosition(def.Position),
		mPosition(def.Position),
		mVelocity(def.Velocity),
		mAcceleration(glm::vec3(0)),
		mMass(def.Mass)
	{
		if (mMass > 0)
		{
			mInvMass = 1 / mMass;
		}
		else
		{
			mInvMass = 0;
		}
	}

	// There is no cleanup required (so far)
	cRigidBody::~cRigidBody() = default;

	void cRigidBody::GetTransform(glm::mat4& transformOut)
	{
		transformOut = glm::translate(glm::mat4(1),mPosition);
	}

	void cRigidBody::ApplyForce(const glm::vec3& force)
	{
		mAcceleration += force * mInvMass;
	}

	void cRigidBody::ApplyImpulse(const glm::vec3& impulse)
	{
		mVelocity += impulse;
	}
}
