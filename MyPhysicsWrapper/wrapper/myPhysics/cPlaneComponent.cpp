#include "cPlaneComponent.h"
#include "phys/Util.h"

nPhysics::cPlaneComponent::cPlaneComponent(sPlaneDef defs)
{
	phys::sRigidBodyDef bodyDef;
	bodyDef.Position = defs.Point;
	bodyDef.Mass = 0.0f; // Planes never move
	float constant = glm::dot(defs.Point, defs.Normal);
	mShape = new phys::cPlane(defs.Normal, constant);
	mRigidBody = new phys::cRigidBody(bodyDef, mShape);
}

nPhysics::cPlaneComponent::~cPlaneComponent()
{
	delete mRigidBody;
	delete mShape;
}

void nPhysics::cPlaneComponent::GetTransform(glm::mat4& transformOut)
{
	mRigidBody->GetTransform(transformOut);
}

void nPhysics::cPlaneComponent::ApplyForce(glm::vec3 force)
{
	mRigidBody->ApplyForce(force);
}

std::string nPhysics::cPlaneComponent::ToString()
{
	return "Plane: Normal: " + glm::to_string(mShape->GetNormal());
}

void nPhysics::cPlaneComponent::ApplyImpulse(glm::vec3 impulse)
{
	mRigidBody->ApplyImpulse(impulse);
}
