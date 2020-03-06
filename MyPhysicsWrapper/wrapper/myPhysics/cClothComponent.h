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
	private:
		phys::cSoftBody *mSoftBody;
		/*
		 * Calculates the linear index of the node to the "right" of the
		 * `idxNode`th node; considering the grid specified in `def`
		 *
		 * Returns false if the `idxNode` is at the right-most column
		 * of the grid
		 */
		bool GetIndexOfRightNode(size_t idxNode, const sClothDef& def, size_t& idxRight);
		/*
		 * Calculates the linear index of the node "below" of the
		 * `idxNode`th node; considering the grid specified in `def`
		 *
		 * Returns false if the `idxNode` is at the bottom-most column
		 * of the grid
		 */
		bool GetIndexOfBelowNode(size_t idxNode, const sClothDef& def, size_t& idxBelow);
	};
}
