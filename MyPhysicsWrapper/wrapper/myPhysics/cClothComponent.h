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
		 * Gets the position of the first Node in the mesh
		 * Retutrns an identity matrix if the mesh has no nodes
		 */
		void GetTransform(glm::mat4& transformOut) override;
		void ApplyForce(glm::vec3 force) override;
		void ApplyImpulse(glm::vec3 impulse) override;
		// Constructs a string containing useful info of the cloth:
		// TODO
		std::string ToString() override;
		size_t NumNodes() override;
		bool GetNodeRadius(size_t index, float& radiusOut) override;
		bool GetNodePosition(size_t index, glm::vec3& posOut) override;
		/*
		 * Gets the positions on the physics world of the ends of the
		 * springs of the mesh as a collection of pairs of 3D vectors
		 */
		bool GetSpringsPositions(std::vector<std::pair<glm::vec3, glm::vec3>>& springsOut) override;
	private:
		phys::cSoftBody *mSoftBody;
	};
}
