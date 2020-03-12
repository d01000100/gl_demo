#include "cSoftBody.h"
#include <sstream>
#include <iostream>
#include "nCollide.h"
#include <game_math.h>

namespace phys
{
	cSoftBody::cNode::cNode(const sSoftBodyNodeDef& def) :
	mSpringForce(glm::vec3(0)),
		mPosition(def.Position),
		mVelocity(glm::vec3(0)),
		mAcceleration(glm::vec3(0)),
		mPreviousPosition(def.Position),
		mRadius(1), // this will be recalculated
		mMass(def.Mass)
	{}

	bool cSoftBody::cNode::IsNeighbor(cNode* other)
	{
		for (auto spring : mAttachedSprings)
		{
			if (spring->getOther(this) == other)
			{
				return true;
			}
		}
		return false;
	}

	void cSoftBody::cNode::CalculateRadius()
	{
		float smallestDistance = std::numeric_limits<float>::max();
		for (auto spring : mAttachedSprings)
		{
			float dist = glm::distance(
				spring->getOther(this)->mPosition, 
				this->mPosition);
			if (dist < smallestDistance)
			{
				smallestDistance = dist;
			}
		}
		// TODO: maybe tweak
		mRadius = smallestDistance * 0.5f;
	}

	void cSoftBody::cNode::ApplyForce(const glm::vec3& force)
	{
		if (mMass != 0)
		{
			mAcceleration += force / mMass;
		}
	}

	void cSoftBody::cNode::Integrate(float deltaTime, const glm::vec3& externalForces)
	{
		if (mMass == 0) { return; }
		// 1. Integrate position and velocity
		// 2) Update the body's previous position.
		mPreviousPosition = mPosition;
		// 3) Do some integrating!
		mVelocity += (mAcceleration + externalForces) * deltaTime;
		mVelocity *= glm::pow(0.80f, deltaTime);  // applying the dampening before we use the velocity
		mPosition += mVelocity * deltaTime;
	}

	cSoftBody::cSpring::cSpring(cNode* nodeA, cNode* nodeB, float springConstant) :
		mNodeA(nodeA),
		mNodeB(nodeB),
		mRestingLength(glm::distance(nodeA->mPosition, nodeB->mPosition)),
		mStiffnessConstant(springConstant),
		mCurrentForceAtoB(glm::vec3(0))
	{
	}

	cSoftBody::cNode* cSoftBody::cSpring::getOther(cNode* node)
	{
		if (mNodeA == node)
		{
			return mNodeB;
		}
		else if (mNodeB == node)
		{
			return mNodeA;
		}
		else
		{
			return nullptr;
		}
	}

	void cSoftBody::cSpring::UpdateSpringForce()
	{
		// HOOKE'S LAW
		// F = k x (difference to position of rest)
		glm::vec3 sep = mNodeB->mPosition - mNodeA->mPosition;
		float dist = glm::length(sep);
		float x = dist - mRestingLength;
		mCurrentForceAtoB = glm::normalize(sep) * x * mStiffnessConstant;
		if (glm::length(mCurrentForceAtoB) < 0.001f)
		{
			mCurrentForceAtoB = glm::vec3(0);
		}
	}

	void cSoftBody::cSpring::ApplyForceToNodes()
	{
		mNodeA->ApplyForce(mCurrentForceAtoB);
		mNodeB->ApplyForce(-mCurrentForceAtoB);
	}

	cSoftBody::cSoftBody(sSoftBodyDef& def) :
		iCollisionBody(eBodyType::soft),
		mWind(glm::vec3(0,0,4))
	{
		size_t numNodes = def.Nodes.size();
		mNodes.resize(numNodes);
		int i = 0;
		for (auto node : def.Nodes)
		{
			mNodes[i] = new cNode(node);
			i++;
		}
		size_t numSprings = def.Springs.size();
		mSprings.resize(numSprings);
		i = 0;
		for (auto Spring : def.Springs)
		{
			mSprings[i] = new cSpring(
				mNodes[def.Springs[i].first],
				mNodes[def.Springs[i].second],
				def.SpringConstant);
			mNodes[def.Springs[i].first]->mAttachedSprings.push_back(mSprings[i]);
			mNodes[def.Springs[i].second]->mAttachedSprings.push_back(mSprings[i]);
			i++;
		}
		std::cout << toString() << std::endl;
		for (auto node : mNodes)
		{
			node->CalculateRadius();
		}
	}

	cSoftBody::~cSoftBody()
	{
		for (auto Spring : mSprings)
		{
			delete Spring;
		}
		for (auto node : mNodes)
		{
			delete node;
		}
	}

	std::string cSoftBody::toString()
	{
		std::stringstream ss;
		ss << "NumNodes: " << numNodes() << std::endl;
		int i = 1;
		for (auto node : mNodes)
		{
			ss << "Node" << i << ": " << glm::to_string(node->mPosition) << std::endl;
			i++;
		}
		return ss.str();
	}

	bool cSoftBody::GetAABB(glm::vec3& mins, glm::vec3& maxs)
	{
		if (mNodes.empty()) { return false; }
		mins = mNodes[0]->mPosition;
		maxs = mNodes[0]->mPosition;
		for (auto node : mNodes)
		{
			// TODO: Add radius and a little extra to detect nearby objects
			auto pos = node->mPosition;
			if (pos.x < mins.x) mins.x = pos.x;
			if (pos.y < mins.y) mins.y = pos.y;
			if (pos.z < mins.z) mins.z = pos.z;
			if (pos.x > maxs.x) maxs.x = pos.x;
			if (pos.y > maxs.y) maxs.y = pos.y;
			if (pos.z > maxs.z) maxs.z = pos.z;
		}
		return true;
	}

	bool cSoftBody::GetNodePosition(size_t index, glm::vec3& positionOut)
	{
		if (index >= mNodes.size()) { return false; }
		positionOut = mNodes[index]->mPosition;
		return true;
	}

	bool cSoftBody::GetNodeRadius(size_t index, float& radiusOut)
	{
		if (index >= mNodes.size()) { return false; }
		radiusOut = mNodes[index]->mRadius;
		return true;
	}

	size_t cSoftBody::numNodes()
	{
		return mNodes.size();
	}

	void cSoftBody::Integrate(float deltaTime, const glm::vec3& gravity)
	{
		mDt = deltaTime;
		// 1. Apply wind
		rotateWind();
		updateInternal(mWind);
		// 2. Apply forces from strings to nodes
		for (auto spring : mSprings)
		{
			spring->UpdateSpringForce();
			spring->ApplyForceToNodes();
		}
		// 3. Integrate nodes
		for (auto node : mNodes)
		{
			node->Integrate(deltaTime, gravity);
		}
		// 4. Do intergral collisions
		for (int i = 0; i < mNodes.size() - 1; i++)
		{
			auto nodeA = mNodes[i];
			for (int j = i + 1; j < mNodes.size(); j++)
			{
				auto nodeB = mNodes[j];
				if (!nodeA->IsNeighbor(nodeB))
				{
					Collide(nodeA, nodeB);
				}
			}
		}
	}

	void cSoftBody::updateInternal(const glm::vec3& wind)
	{
		for (auto node : mNodes)
		{
			node->ApplyForce(wind);
		}
	}

	void cSoftBody::ClearAccelerations()
	{
		for (auto node : mNodes)
		{
			node->mAcceleration = glm::vec3(0);
		}
	}

	bool cSoftBody::Collide(cNode* nodeA, cNode* nodeB)
	{
		glm::vec3 cA = nodeA->mPreviousPosition;
		glm::vec3 cB = nodeB->mPreviousPosition;
		glm::vec3 vA = nodeA->mPosition - nodeA->mPreviousPosition;
		glm::vec3 vB = nodeB->mPosition - nodeB->mPreviousPosition;
		float rA = nodeA->mRadius;
		float rB = nodeB->mRadius;
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
		float ma = nodeA->mMass;
		float mb = nodeB->mMass;
		float mt = ma + mb;
		if (result == -1)
		{
			//std::cout << "Sphere clipping sphere " << std::endl;
			// Already Colliding at the beggining of the timestep
			// Calculate the impulse to set them apart
			float initialDistance = glm::distance(nodeA->mPreviousPosition, nodeB->mPreviousPosition);
			float targetDistance = rA + rB;
			glm::vec3 impulseToA = glm::normalize(nodeA->mPreviousPosition - nodeB->mPreviousPosition);
			impulseToA *= (targetDistance - initialDistance);
			// rewind time
			nodeA->mPosition = nodeA->mPreviousPosition;
			nodeB->mPosition = nodeB->mPreviousPosition;
			// apply the impulse considering the mass of each sphere
			nodeA->mVelocity += impulseToA * (mb / mt);
			nodeB->mVelocity -= impulseToA * (ma / mt);
			// re integrate
			nodeA->Integrate(mDt);
			nodeB->Integrate(mDt);
			return true;
		}
		// Collided during the timestep

		//std::cout << "Sphere colliding sphere." << std::endl;
		// rewind time to the point of collision
		nodeA->mPosition = nodeA->mPreviousPosition + vA * t;
		nodeB->mPosition = nodeB->mPreviousPosition + vB * t;

		vA = nodeA->mVelocity;
		vB = nodeB->mVelocity;

		float c = 0.8; // how much energy will they loose
		// float c = nodeA->elasticCoefficent * nodeB->elasticCoefficient
		// calculate collided velocities (wikipedia)
		nodeA->mVelocity = (c * mb * (vB - vA) + ma * vA + mb * vB) / mt;
		nodeB->mVelocity = (c * ma * (vA - vB) + ma * vA + mb * vB) / mt;

		// re integrate the rest of the timestep
		nodeA->Integrate(mDt * (1.f - t));
		nodeB->Integrate(mDt * (1.f - t));
		return true;
	}

	void cSoftBody::rotateWind()
	{
		mWind = glm::rotate(
			mWind, 
			glm::radians(10.0f) * mDt, 
			glm::vec3(0, 1, 0)
		);
	}
}
