#include "cWorld.h"    // My header
#include "nCollide.h"  // collision detection functions from
#include <iostream>

// REAL-TIME COLLISION DETECTION, ERICSON

namespace phys
{
	cWorld::cWorld() :
		mDt(1.f / 60.f), // assume 60 fps simulation
		mIntegrator(cIntegrator()),
		mGravity(glm::vec3(0,-1,0)), // "downward" gravity
		mBodies(std::vector<cRigidBody*>()) // empty vector of rigid bodies
	{
	}

	// All properties are heap based,
	// so there's no need to clean them
	cWorld::~cWorld() = default;
	
	void cWorld::GetGravity(glm::vec3& gravityOut)
	{
		gravityOut = mGravity;
	}

	void cWorld::SetGravity(const glm::vec3& gravityIn)
	{
		mGravity = gravityIn;
	}

	void cWorld::Update(float dt)
	{
		mDt = dt;
		// 1) If we have no bodies, there's nothing to do... return.
		if (mBodies.empty()) { return; }
		// 2) Integrate each body.
		auto itBodies = mBodies.begin();
		for (;itBodies != mBodies.end();++itBodies)
		{
			IntegrateRigidBody(*itBodies,dt);
		}
		// 3) Perform collision handling on each unique pair of bodies.
		for (int body1 = 0; body1 < mBodies.size() - 1; body1++) {
            for (int body2 = body1 + 1; body2 < mBodies.size(); body2++) {
                Collide(mBodies[body1], mBodies[body2]);
            }
        }
		// 4) Clear the acceleration of each rigid body.
		for (itBodies = mBodies.begin();itBodies != mBodies.end();++itBodies)
		{
			(*itBodies)->mAcceleration = glm::vec3(0);
		}
	}

	bool cWorld::AddRigidBody(cRigidBody* rigidBody)
	{
		// 1) Null check
		if (rigidBody == nullptr)  {return false;}
		// 2) Check if we currently have this rigid body.
		if (std::find(mBodies.begin(), mBodies.end(), rigidBody) != mBodies.end())
		{
		//    If yes: Return false to indicate nothing was not added.
			return false;
		}
		//    If no:  Add it, then return true to indicate it was added.
		mBodies.push_back(rigidBody);
		return true; 
	}

	bool cWorld::RemoveRigidBody(cRigidBody* rigidBody)
	{
		// 1) Null check
		if (rigidBody == nullptr) { return false; }
		// 2) Check if we currently have this rigid body.
		auto itToDelete = std::find(mBodies.begin(), mBodies.end(), rigidBody);
		if (itToDelete != mBodies.end())
		{
		//    If yes: Remove it, then return true to indicate it was removed.
			mBodies.erase(itToDelete);
			return true;
		}
		else
		//    If no:  Return false to indicate nothing was removed.
		{
			return false;
		}
	}

	void cWorld::IntegrateRigidBody(cRigidBody* body, float dt)
	{
		// 1) Static bodies are not to be integrated!
		if (body->IsStatic()) return;
		// 2) Update the body's previous position.
        body->mPreviousPosition = body->mPosition;
		// 3) Do some integrating!
        mIntegrator.RK4(body->mPosition, body->mVelocity, body->mAcceleration, mGravity, dt);
		// 4) Apply some linear damping ???
		body->mVelocity *= glm::pow(0.95f, dt);
	}
	bool cWorld::Collide(cRigidBody* bodyA, cRigidBody* bodyB)
	{
		eShapeType shapeTypeA = bodyA->GetShapeType();
        eShapeType shapeTypeB = bodyB->GetShapeType();
        if (shapeTypeA == phys::eShapeType::sphere &&
            shapeTypeB == phys::eShapeType::sphere) 
        {
            return CollideSphereSphere(
				bodyA, 
				dynamic_cast<cSphere*>(bodyA->GetShape()), 
				bodyB, 
				dynamic_cast<cSphere*>(bodyB->GetShape())
			);
        }
        else if (shapeTypeA == phys::eShapeType::sphere &&
                 shapeTypeB == phys::eShapeType::plane) 
        {
            return CollideSpherePlane(
				bodyA, 
				dynamic_cast<cSphere*>(bodyA->GetShape()), 
				bodyB, 
				dynamic_cast<cPlane*>(bodyB->GetShape())
			);
        }
        else if (shapeTypeA == phys::eShapeType::plane &&
                 shapeTypeB == phys::eShapeType::sphere) 
        {
            return CollideSpherePlane(
				bodyB, 
				dynamic_cast<cSphere*>(bodyB->GetShape()),
				bodyA, 
				dynamic_cast<cPlane*>(bodyA->GetShape())
			);
        }
        // If we don't support the collision then bail
        else { return false; }
	}

	bool cWorld::CollideSpherePlane(cRigidBody* sphereBody, cSphere* sphereShape,
		cRigidBody* planeBody, cPlane* planeShape)
	{
	    glm::vec3 c = sphereBody->mPreviousPosition;
        float r = sphereShape->GetRadius();
        glm::vec3 v = sphereBody->mPosition - sphereBody->mPreviousPosition,
          n = planeShape->GetNormal();
        float d = planeShape->GetConstant();
        float t(0.0f);
        glm::vec3 q; // point in the plane
        // Chapter 5.5 of Ericson's book
        int result = nCollide::intersect_moving_sphere_plane(c, r, v, n, d, t, q);
        // closestPointOnPLane
        if (result == 0)
        {
            //no collision
            return false;
        }
        if (result == -1)
        {
            //already colliding at the begining of the timeStep
            glm::vec3 pointOnPlane = nCollide::closest_point_on_plane(
                sphereBody->mPreviousPosition,
                planeShape->GetNormal(),
                planeShape->GetConstant()
            );
        	std::cout << "Sphere clipping plane in: " << glm::to_string(pointOnPlane) << std::endl;
            // calculate how much distance we clipped into the plane
            float distance = glm::distance(sphereBody->mPreviousPosition, pointOnPlane);
            float targetDistance = r;
            // calculate how much force do we need to get out from the plane
            glm::vec3 impulse = n * (targetDistance - distance) / mDt;
            // rewind time
            sphereBody->mPosition = sphereBody->mPreviousPosition;
            // apply the impulse
            sphereBody->mVelocity += impulse;
            // re integrate
            IntegrateRigidBody(sphereBody, mDt);
            return true;
        }
        std::cout << "Sphere colliding plane in: " << glm::to_string(q) << std::endl;

        sphereBody->mVelocity = glm::reflect(sphereBody->mVelocity, planeShape->GetNormal());
        // To take out energy from the collision
        // we take down a little bit from the reflected direction 
        // in the direction of the normal. (use the vector projection for that)
        glm::vec3 nComponent = glm::proj(sphereBody->mVelocity, planeShape->GetNormal());
        sphereBody->mVelocity -= nComponent * 0.2f;

        // We collided somewhere in the middle of the timestep
        // We need to reintegrate the step, with the new velocity, which is reflected
        sphereBody->mPosition = c + v * t;
        IntegrateRigidBody(sphereBody, mDt * (1.f - t));
        return true;
	}

	bool cWorld::CollideSphereSphere(cRigidBody* bodyA, cSphere* shapeA, 
		cRigidBody* bodyB, cSphere* shapeB)
	{
		glm::vec3 cA = bodyA->mPreviousPosition;
        glm::vec3 cB = bodyB->mPreviousPosition;
        glm::vec3 vA = bodyA->mPosition - bodyA->mPreviousPosition;
        glm::vec3 vB = bodyB->mPosition - bodyB->mPreviousPosition;
        float rA = shapeA->GetRadius();
        float rB = shapeB->GetRadius();
        float t(0.f);
        
        // Chapter 5.5 of Ericson's book
        int result = nCollide::intersect_moving_sphere_sphere(
            cA, rA, vA, cB, rB, vB, t
        );
        if (result == 0)
        {
            //Not colliding
            return false;
        }
        // get the masses
        float ma = bodyA->mMass;
        float mb = bodyB->mMass;
        float mt = ma + mb;
        if (result == -1)
        {
        	std::cout << "Sphere clipping sphere " << std::endl;
            // Already Colliding at the beggining of the timestep
            // Calculate the impulse to set them apart
            float initialDistance = glm::distance(bodyA->mPreviousPosition, bodyB->mPreviousPosition);
            float targetDistance = shapeA->GetRadius() + shapeB->GetRadius();
            glm::vec3 impulseToA = glm::normalize(bodyA->mPreviousPosition - bodyB->mPreviousPosition);
            impulseToA *= (targetDistance -  initialDistance);
            // rewind time
            bodyA->mPosition = bodyA->mPreviousPosition;
            bodyB->mPosition = bodyB->mPreviousPosition;
            // apply the impulse considering the mass of each sphere
            bodyA->mVelocity += impulseToA * (mb / mt);
            bodyB->mVelocity -= impulseToA * (ma / mt);
            // re integrate
            IntegrateRigidBody(bodyA, mDt);
            IntegrateRigidBody(bodyB, mDt);
            return true;
        }
        // Collided during the timestep
        
        std::cout << "Sphere colliding sphere." << std::endl;
        // rewind time to the point of collision
        bodyA->mPosition = bodyA->mPreviousPosition + vA * t;
        bodyB->mPosition = bodyB->mPreviousPosition + vB * t;

        vA = bodyA->mVelocity;
        vB = bodyB->mVelocity;

        float c = 0.8; // how much energy will they loose
        // TODO: Use the damping of each object
        // float c = bodyA->elasticCoefficent * bodyB->elasticCoefficient
        // calculate collided velocities (wikipedia)
        bodyA->mVelocity = (c*mb*(vB - vA) + ma*vA + mb*vB) / mt;
        bodyB->mVelocity = (c*ma*(vA - vB) + ma*vA + mb*vB) / mt;

        // re integrate the rest of the timestep
        IntegrateRigidBody(bodyA, mDt * (1.f - t));
        IntegrateRigidBody(bodyB, mDt * (1.f - t));
        return true;
	}
}