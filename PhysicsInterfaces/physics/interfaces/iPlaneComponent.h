#pragma once
#include <game_math.h>
#include "iPhysicsComponent.h"

namespace nPhysics
{
	struct sPlaneDef
	{
		glm::vec3 Point;
		glm::vec3 Normal;
	};
	class iPlaneComponent : public iPhysicsComponent
	{
	public:
		virtual ~iPlaneComponent() {}

	protected:
		iPlaneComponent() : iPhysicsComponent(eComponentType::plane) {}
	private:
		iPlaneComponent(const iPlaneComponent& other) = delete;
		iPlaneComponent& operator=(const iPlaneComponent& other) = delete;
	};
}