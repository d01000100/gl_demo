#include "cPhysicsComponent.h"
#include  <iostream>

// All properties are heap based. No cleaning necessary.
nPhysics::cPhysicsComponent::~cPhysicsComponent() = default;

void nPhysics::cPhysicsComponent::GetTransform(glm::mat4& transformOut)
{
	mRigidBody.GetTransform(transformOut);
}

nPhysics::cPhysicsComponent::cPhysicsComponent(
	phys::sRigidBodyDef bodyDef,
	phys::iShape* pShape) :
	mRigidBody(bodyDef,pShape)
{
	switch (pShape->GetShapeType())
	{
	case phys::eShapeType::plane:
		break;
	case phys::eShapeType::sphere:
		break;
	default:
		printf("Creating a physics component with an invalid component type.");
	}
}
