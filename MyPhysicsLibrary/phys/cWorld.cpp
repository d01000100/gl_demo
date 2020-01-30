#include "cWorld.h"    // My header
#include "nCollide.h"  // collision detection functions from
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
		// 3) TODO: Perform collision handling on each unique pair of bodies.
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
	}
	bool cWorld::Collide(cRigidBody* bodyA, cRigidBody* bodyB)
	{
		// TODO:
		// 
		// 1) Based on shape type, determine which specific collision handling
		//    method to use.
		// 2) Cast up the shapes, call the methods, return the result.
		return false;
	}

	bool cWorld::CollideSpherePlane(cRigidBody* sphereBody, cSphere* sphereShape,
		cRigidBody* planeBody, cPlane* planeShape)
	{
		// TODO:
		// 
		// From our textbook, REAL-TIME COLLISION DETECTION, ERICSON
		// Use intersect_moving_sphere_plane(...inputs...outputs...)
		// to determine if:
		// case A: The sphere did not collide during the timestep.
		// case B: The sphere was already colliding at the beginning of the timestep.
		// case C: The sphere collided during the timestep.
		//
		// case A: Return false to indicate no collision happened.
		//
		// case B: Do the timestep again for this sphere after applying an
		//         impulse that should separate it from the plane.
		// 
		// 1) From our textbook, use closest_point_on_plane(..inputs..) to determine the 
		//    penetration-depth of the sphere at the beginning of the timestep.
		//    (This penetration-depth is the distance the sphere must travel during
		//    the timestep in order to escape the plane.)
		// 2) Use the sphere's center and the closest point on the plane to define
		//    the direction of our impulse vector.
		// 3) Use (penetration-depth / DT) to define the magnitude of our impulse vector.
		//    (The impulse vector is now distance/time ...a velocity!)
		// 4) Apply the impulse vector to sphere velocity.
		// 5) Reset the sphere position.
		// 6) Re-do the integration for the timestep.
		// 7) Return true to indicate a collision has happened.
		// 
		// 
		// case C: Rewind to the point of impact, reflect, then replay.
		//
		// 1) Use the outputs from the Ericson function to determine
		//    and set the sphere position to the point of impact.
		// 2) Reflect the sphere's velocity about the plane's normal vector.
		// 3) Apply some energy loss (to the velocity) in the direction
		//    of the plane's normal vector.
		// 4) Re-integrate the sphere with its new velocity over the remaining
		//    portion of the timestep.
		// 5) Return true to indicate a collision has happened.
		
		return false; // placeholder
	}

	bool cWorld::CollideSphereSphere(cRigidBody* bodyA, cSphere* shapeA, 
		cRigidBody* bodyB, cSphere* shapeB)
	{
		// TODO:
		// 
		// From our textbook, REAL-TIME COLLISION DETECTION, ERICSON
		// Use intersect_moving_sphere_sphere(...inputs...outputs...)
		// to determine if:
		// case A: The spheres don't collide during the timestep.
		// case B: The spheres were already colliding at the beginning of the timestep.
		// case C: The spheres collided during the timestep.
		//
		// case A: Return false to indicate no collision happened.
		//
		// case B: Do the timestep again for these spheres after
		//         applying an impulse that should separate them.
		// 
		// 1) Determine the penetration-depth of the spheres at the beginning of the timestep.
		//    (This penetration-depth is the distance the spheres must travel during
		//    the timestep in order to separate.)
		// 2) Use the sphere's centers to define the direction of our impulse vector.
		// 3) Use (penetration-depth / DT) to define the magnitude of our impulse vector.
		//    (The impulse vector is now distance/time ...a velocity!)
		// 4) Apply a portion of the impulse vector to sphereA's velocity.
		// 5) Apply a portion of the impulse vector to sphereB's velocity.
		//    (Be sure to apply the impulse in opposing directions.)
		// 6) Reset the spheres' positions.
		// 7) Re-do the integration for the timestep.
		// 8) Return true to indicate a collision has happened.
		// 
		// 
		// case C: 
		//
		// 1) Use the outputs from the Ericson function to determine
		//    and set the spheres positions to the point of impact.
		// 2) Use the inelastic collision response equations from
		//    Wikepedia to set they're velocities post-impact.
		// 3) Re-integrate the spheres with their new velocities
		//    over the remaining portion of the timestep.
		// 4) Return true to indicate a collision has happened.
		
		return false; // placeholder
	}
}