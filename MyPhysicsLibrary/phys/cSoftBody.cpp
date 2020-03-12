#include "cSoftBody.h"
#include <sstream>
#include <iostream>

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
		iCollisionBody(eBodyType::soft)
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

	void cSoftBody::Integrate(float deltaTime, const glm::vec3& gravity, const glm::vec3& wind)
	{
		// 1. Apply gracity and wind
		updateInternal(deltaTime, gravity, wind);
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
					// Collide
				}
			}
		}
	}

	void cSoftBody::updateInternal(float dt, const glm::vec3& gravity, const glm::vec3& wind)
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
}
