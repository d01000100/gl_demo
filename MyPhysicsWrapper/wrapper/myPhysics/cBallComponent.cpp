#include "cBallComponent.h"
#include "phys/Util.h"

nPhysics::cBallComponent::cBallComponent(nPhysics::sBallDef defs) 
{
	mShape = new phys::cSphere(defs.Radius);
	phys::sRigidBodyDef bodyDef;
	bodyDef.Mass = defs.Mass;
	bodyDef.Position = defs.Position;
	mRigidBody = new phys::cRigidBody(bodyDef, mShape);
}

nPhysics::cBallComponent::~cBallComponent()
{
	delete mRigidBody;
	delete mShape;
}

void nPhysics::cBallComponent::GetTransform(glm::mat4& transformOut)
{
	mRigidBody->GetTransform(transformOut);
}

std::string nPhysics::cBallComponent::ToString()
{
	return phys::Util::RigidBodyToString(*mRigidBody);
}

void nPhysics::cBallComponent::ApplyForce(glm::vec3 force)
{
	mRigidBody->ApplyForce(force);
}

void nPhysics::cBallComponent::ApplyImpulse(glm::vec3 impulse)
{
	mRigidBody->ApplyImpulse(impulse);
}
