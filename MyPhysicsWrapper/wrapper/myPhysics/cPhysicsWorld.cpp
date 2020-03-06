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
		cClothComponent* pClothComponent = nullptr;
		switch (component->GetComponentType())
		{
		case eComponentType::ball:
			pBallComponent = dynamic_cast<cBallComponent*>(component);
			return mWorld.AddBody(pBallComponent->mRigidBody);
		case eComponentType::plane:
			pPlaneComponent = dynamic_cast<cPlaneComponent*>(component);
			return mWorld.AddBody(pPlaneComponent->mRigidBody);
		case eComponentType::cloth:
			pClothComponent = dynamic_cast<cClothComponent*>(component);
			return mWorld.AddBody(pClothComponent->mSoftBody);
		default:
			printf("cPhysicsWorld::AddComponent doesn't recognize the component type\n");
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
	cClothComponent* pClothComponent = nullptr;
	switch (component->GetComponentType())
	{
	case eComponentType::ball:
		pBallComponent = dynamic_cast<cBallComponent*>(component);
		return mWorld.RemoveBody(pBallComponent->mRigidBody);
	case eComponentType::plane:
		pPlaneComponent = dynamic_cast<cPlaneComponent*>(component);
		return mWorld.RemoveBody(pPlaneComponent->mRigidBody);
	case eComponentType::cloth:
		pClothComponent = dynamic_cast<cClothComponent*>(component);
		return mWorld.RemoveBody(pClothComponent->mSoftBody);
	default:
		printf("cPhysicsWorld::RemoveComponent doesn't recognize the component type\n");
		return false;
	}
}
