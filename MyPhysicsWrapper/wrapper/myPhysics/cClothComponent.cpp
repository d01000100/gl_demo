#include "cClothComponent.h"
#include <iostream>

void nPhysics::cClothComponent::GetTransform(glm::mat4& transformOut)
{
	glm::vec3 firstNodePos;
	if (mSoftBody->GetNodePosition(0, firstNodePos))
	{
		transformOut = glm::translate(glm::mat4(1), firstNodePos);
	}
	else
	{
		transformOut = glm::mat4(1);
	}
}

void nPhysics::cClothComponent::ApplyForce(glm::vec3 force)
{
}

void nPhysics::cClothComponent::ApplyImpulse(glm::vec3 impulse)
{
}

std::string nPhysics::cClothComponent::ToString()
{
	// TODO: ClothComponent ToString
	return "TODO Cloth toString";
}

size_t nPhysics::cClothComponent::NumNodes()
{
	return mSoftBody->numNodes();
}

bool nPhysics::cClothComponent::GetNodeRadius(size_t index, float& radiusOut)
{
	return mSoftBody->GetNodeRadius(index, radiusOut);
}

bool nPhysics::cClothComponent::GetNodePosition(size_t index, glm::vec3& posOut)
{
	return mSoftBody->GetNodePosition(index, posOut);
}

bool nPhysics::cClothComponent::GetIndexOfRightNode(
	size_t idxNode, 
	const sClothDef& def,
    size_t& idxRight)
{
	// TODO: Actually see if I need them
	return false;
}

bool nPhysics::cClothComponent::GetIndexOfBelowNode(
	size_t idxNode, 
	const sClothDef& def,
    size_t& idxBelow)
{
	return false;
}

nPhysics::cClothComponent::cClothComponent(const sClothDef& def)
{
	phys::sSoftBodyDef softDef;
	softDef.SpringConstant = def.SpringConstant;

	size_t numNodesTotal = def.NumNodesAcross * def.NumNodesDown;
	softDef.Nodes.resize(numNodesTotal);
	
	// vector in the direction from A to B
	glm::vec3 sepAcross = def.CornerB - def.CornerA;
	// that has the lenght of the separation between vectors
	if (def.NumNodesDown > 1)
	{
		sepAcross /= def.NumNodesAcross - 1;
	}
	std::cout << "Sep across: " << glm::length(sepAcross) << std::endl;
	// vector in the down direction 
	// that has the lenght of the separation between vectors
	glm::vec3 sepDown = glm::normalize(def.DownDirection) * glm::length(sepAcross);
	
	size_t idxNode = 0;
	for (size_t idxDown = 0; idxDown < def.NumNodesDown; idxDown++)
	{
		for (size_t idxAcross = 0; idxAcross < def.NumNodesAcross; idxAcross++, idxNode++)
		{
			// calculate the position by multiplicatin the coordinates
			// by the "unit" vectors across and down
			softDef.Nodes[idxNode].Position = def.CornerA +
				(float)idxAcross * sepAcross +
				(float)idxDown * sepDown;
			if (idxNode == 0 || // The "first" node; CornerA
				idxNode == def.NumNodesAcross - 1) // CornerB
			{
				// We don't want the corner nodes to move.
				softDef.Nodes[idxNode].Mass = 0;
			}
			else
			{
				softDef.Nodes[idxNode].Mass = def.NodeMass;
			}
			// Am I to the left of some other node?
			if (idxAcross < def.NumNodesAcross - 1)
			{
				softDef.Springs.emplace_back(std::pair<size_t, size_t>(idxNode, idxNode + 1));
			}
			// Am I above some other node?
			if (idxDown < def.NumNodesDown - 1)
			{
				softDef.Springs.emplace_back(std::pair<size_t, size_t>(idxNode, idxNode + def.NumNodesAcross));
			}
		}
	}
	mSoftBody = new phys::cSoftBody(softDef);
}

nPhysics::cClothComponent::~cClothComponent()
{
	delete mSoftBody;
	mSoftBody = nullptr;
}
