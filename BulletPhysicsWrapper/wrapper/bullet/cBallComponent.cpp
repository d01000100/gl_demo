#include "cBallComponent.h"
#include "nConvert.h"

nPhysics::cBallComponent::cBallComponent(nPhysics::sBallDef defs) 
{
	mShape = new btSphereShape(defs.Radius);
	
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(nConvert::toBullet(defs.Position));
	btDefaultMotionState *motionState = new btDefaultMotionState(startTransform);

	bool isDynamic = (defs.Mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		mShape->calculateLocalInertia(defs.Mass, localInertia);
	
	btRigidBody::btRigidBodyConstructionInfo bodyDef(
		defs.Mass,
		motionState,
		mShape,
		localInertia
	);
	bodyDef.m_restitution = 0.8f; // how much energy it losses when it collides
	
	mRigidBody = new btRigidBody(bodyDef);
}

nPhysics::cBallComponent::~cBallComponent()
{
	delete mRigidBody->getCollisionShape();
	delete mRigidBody->getMotionState();
	delete mRigidBody;
	delete mShape;
}

void nPhysics::cBallComponent::GetTransform(glm::mat4& transformOut)
{
	btTransform transform;
	mRigidBody->getMotionState()->getWorldTransform(transform);
	nConvert::toGLM(transform, transformOut);
}

std::string nPhysics::cBallComponent::ToString()
{
	return "bullet sphere. todo thing";
}

void nPhysics::cBallComponent::ApplyForce(glm::vec3 force)
{
	mRigidBody->activate(true);
	mRigidBody->applyCentralForce(nConvert::toBullet(force));
}

void nPhysics::cBallComponent::ApplyImpulse(glm::vec3 impulse)
{
	mRigidBody->activate(true);
	mRigidBody->applyCentralImpulse(nConvert::toBullet(impulse));
}
