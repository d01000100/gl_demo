#pragma once
#include "phys/phys.h"
#include "cPhysicsWorld.h"
#include "physics/interfaces/iClothComponent.h"

namespace nPhysics
{
	class cClothComponent : public iClothComponent
	{
		friend cPhysicsWorld;
	public:
		cClothComponent(const sClothDef& def);
		~cClothComponent() override;
		/*
		 * Inherited from iPhysicsComponent.
		 * Not really necessary for cloths.
		 * Returns an identity matrix
		 */
		void GetTransform(glm::mat4& transformOut) override;
		void ApplyForce(glm::vec3 force) override;
		void ApplyImpulse(glm::vec3 impulse) override;
		// Constructs a string containing useful info of the cloth:
		// TODO
		std::string ToString() override;
	private:
		phys::cSoftBody *mSoftBody;
		size_t NumNodes() override;
		bool GetNodeRadius(size_t index, float& radiusOut) override;
		bool GetNodePosition(size_t index, glm::vec3& posOut) override;
	};
}
