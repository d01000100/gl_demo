#include "cPhysicsWorld.h"
#include "includes.h"

using namespace nPhysics;

void nPhysics::cPhysicsWorld::Update(float dt)
{
	mWorld.Update(dt);
}

bool nPhysics::cPhysicsWorld::AddComponent(iPhysicsComponent* component)
{
	if (component)
	{
		cBallComponent* pBallComponent = nullptr;
		cPlaneComponent* pPlaneComponent = nullptr;
		switch (component->GetComponentType())
		{
		case eComponentType::ball:
			pBallComponent = dynamic_cast<cBallComponent*>(component);
			return mWorld.AddRigidBody(pBallComponent->mRigidBody);
		case eComponentType::plane:
			pPlaneComponent = dynamic_cast<cPlaneComponent*>(component);
			return mWorld.AddRigidBody(pPlaneComponent->mRigidBody);
		default:
			printf("cPhysicsWorld doesn't recognize the component type\n");
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool nPhysics::cPhysicsWorld::RemoveComponent(iPhysicsComponent* component)
{
	cBallComponent* pBallComponent = nullptr;
	cPlaneComponent* pPlaneComponent = nullptr;
	switch (component->GetComponentType())
	{
	case eComponentType::ball:
		pBallComponent = dynamic_cast<cBallComponent*>(component);
		return mWorld.RemoveRigidBody(pBallComponent->mRigidBody);
	case eComponentType::plane:
		pPlaneComponent = dynamic_cast<cPlaneComponent*>(component);
		return mWorld.RemoveRigidBody(pPlaneComponent->mRigidBody);
	default:
		printf("cPhysicsWorld doesn't recognize the component type\n");
		return false;
	}
}
