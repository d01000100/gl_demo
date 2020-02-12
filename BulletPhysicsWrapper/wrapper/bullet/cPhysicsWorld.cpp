#include "cPhysicsWorld.h"
#include "includes.h"

using namespace nPhysics;

void nPhysics::cPhysicsWorld::Update(float dt)
{
	mWorld->stepSimulation(dt, 10); 
}

cPhysicsWorld::cPhysicsWorld()
{
	mCollisions = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	mCollisionDispatcher = new btCollisionDispatcher(mCollisions);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	mOverlappingPairs = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	mConstraints = new btSequentialImpulseConstraintSolver;

	mWorld = new btDiscreteDynamicsWorld(
		mCollisionDispatcher, 
		mOverlappingPairs, 
		mConstraints, 
		mCollisions
	);

	mWorld->setGravity(btVector3(0, -10, 0));
}


cPhysicsWorld::~cPhysicsWorld()
{
	delete mWorld;
	delete mConstraints;
	delete mOverlappingPairs;
	delete mCollisionDispatcher;
	delete mCollisions;
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
			mWorld->addRigidBody(pBallComponent->mRigidBody);
			return true;
		case eComponentType::plane:
			pPlaneComponent = dynamic_cast<cPlaneComponent*>(component);
			mWorld->addRigidBody(pPlaneComponent->mRigidBody);
			return true;
		default:
			printf("cPhysicsWorld (bullet) doesn't recognize the component type\n");
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
		mWorld->removeRigidBody(pBallComponent->mRigidBody);
		return true;
	case eComponentType::plane:
		pPlaneComponent = dynamic_cast<cPlaneComponent*>(component);
		mWorld->removeRigidBody(pPlaneComponent->mRigidBody);
		return true;
	default:
		printf("cPhysicsWorld (bullet) doesn't recognize the component type\n");
		return false;
	}
}
