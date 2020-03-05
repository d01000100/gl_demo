#include "cClothComponent.h"

void nPhysics::cClothComponent::GetTransform(glm::mat4& transformOut)
{
	transformOut = glm::mat4(1);
}

void nPhysics::cClothComponent::ApplyForce(glm::vec3 force)
{
}

void nPhysics::cClothComponent::ApplyImpulse(glm::vec3 impulse)
{
}

std::string nPhysics::cClothComponent::ToString()
{
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

nPhysics::cClothComponent::cClothComponent(const sClothDef& def)
{
}

nPhysics::cClothComponent::~cClothComponent()
{
	delete mSoftBody;
}
