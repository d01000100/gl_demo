#include "cSoftBody.h"

namespace phys
{
	cSoftBody::cNode::cNode(const sSoftBodyNodeDef& def) :
	SpringForce(glm::vec3(0)),
		Position(def.Position),
		Velocity(glm::vec3(0)),
		Acceleration(glm::vec3(0)),
		Radius(1), // this will be recalculated
		Mass(def.Mass)
	{}

	bool cSoftBody::cNode::IsNeighbor(cNode* other)
	{
		for (auto spring : AttachedSprings)
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
		for (auto spring : AttachedSprings)
		{
			float dist = glm::distance(
				spring->getOther(this)->Position, 
				this->Position);
			if (dist < smallestDistance)
			{
				smallestDistance = dist;
			}
		}
		// TODO: maybe tweak
		Radius = smallestDistance * 0.5f;
	}

	cSoftBody::cSpring::cSpring(cNode* nodeA, cNode* nodeB, float springConstant) :
		NodeA(nodeA),
		NodeB(nodeB),
		RestingLength(glm::distance(nodeA->Position, nodeB->Position)),
		StiffnessConstant(springConstant)
	{
	}

	cSoftBody::cNode* cSoftBody::cSpring::getOther(cNode* node)
	{
		if (NodeA == node)
		{
			return NodeB;
		}
		else if (NodeB == node)
		{
			return NodeA;
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
		glm::vec3 sep = NodeB->Position - NodeA->Position;
		float dist = glm::length(sep);
		float x = glm::abs(dist - RestingLength);
		CurrentForceAtoB = glm::normalize(sep) * x * StiffnessConstant;
	}

	void cSoftBody::cSpring::ApplyForceToNodes()
	{
		if (NodeB->Mass == 0)
		{
			NodeB->Acceleration += CurrentForceAtoB / NodeB->Mass;
		}
		if (NodeB->Mass == 0)
		{
			NodeB->Acceleration -= CurrentForceAtoB / NodeA->Mass;
		}
	}

	void cSoftBody::IntegrateNode(cNode* node)
	{
		if (node->Mass == 0) { return; }
		// 1. Integrate position and velocity
		// 2. Calculate spring forces??
	}

	cSoftBody::cSoftBody(sSoftBodyDef& def)
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
			i++;
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

	bool cSoftBody::GetAABB(glm::vec3& mins, glm::vec3& maxs)
	{
		if (mNodes.empty()) { return false; }
		mins = mNodes[0]->Position;
		maxs = mNodes[0]->Position;
		for (auto node : mNodes)
		{
			auto pos = node->Position;
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
		positionOut = mNodes[index]->Position;
		return true;
	}

	bool cSoftBody::GetNodeRadius(size_t index, float& radiusOut)
	{
		if (index >= mNodes.size()) { return false; }
		radiusOut = mNodes[index]->Radius;
		return true;
	}

	size_t cSoftBody::numNodes()
	{
		return mNodes.size();
	}

	void cSoftBody::Integrate(float deltaTime)
	{
	}

	void cSoftBody::updateInternal(float dt, const glm::vec3& gravity, const glm::vec3& wind)
	{
	}
}
