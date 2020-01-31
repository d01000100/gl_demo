#pragma once
#include <game_math.h>
#include "iPhysicsComponent.h"

namespace nPhysics
{
	struct sBallDef
	{
		float Mass;
		float Radius;
		glm::vec3 Position;
		sBallDef() :
			Mass(1.f),
			Radius(1.f),
			Position(glm::vec3(0)) {}
	};

	class iBallComponent : public iPhysicsComponent
	{
	public:
		virtual ~iBallComponent() {}
		
	protected:
		iBallComponent() : iPhysicsComponent(eComponentType::ball) {}
	private:
		iBallComponent(const iBallComponent& other) = delete;
		iBallComponent& operator=(const iBallComponent& other) = delete;
	};
}