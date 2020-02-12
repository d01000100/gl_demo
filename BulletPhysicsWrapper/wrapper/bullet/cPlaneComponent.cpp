#include "cPlaneComponent.h"
#include "nConvert.h"

nPhysics::cPlaneComponent::cPlaneComponent(sPlaneDef defs)
{
	float C = glm::dot(defs.Normal, defs.Point);
	mShape = new btStaticPlaneShape(
		nConvert::toBullet(defs.Normal),
		C
	);

	btTransform startTransform;
	startTransform.setIdentity();
	//startTransform.setOrigin(nConvert::toBullet(defs.Point));
	btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);
	
	btRigidBody::btRigidBodyConstructionInfo bodyDef(
		0.0f,
		motionState,
		mShape,
		btVector3(0, 0, 0)
	);
	bodyDef.m_restitution = 1.0f;
	
	mRigidBody = new btRigidBody(bodyDef);
}

nPhysics::cPlaneComponent::~cPlaneComponent()
{
	delete mRigidBody->getCollisionShape();
	delete mRigidBody->getMotionState();
	delete mRigidBody;
	delete mShape;
}

void nPhysics::cPlaneComponent::GetTransform(glm::mat4& transformOut)
{
	btTransform transform;
	mRigidBody->getMotionState()->getWorldTransform(transform);
	nConvert::toGLM(transform, transformOut);
}

void nPhysics::cPlaneComponent::ApplyForce(glm::vec3 force)
{
	mRigidBody->applyCentralForce(nConvert::toBullet(force));
}

std::string nPhysics::cPlaneComponent::ToString()
{
	return "Bullet Plane. TODO toString";
}

void nPhysics::cPlaneComponent::ApplyImpulse(glm::vec3 impulse)
{
	mRigidBody->applyCentralImpulse(nConvert::toBullet(impulse));
}
