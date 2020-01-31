#pragma once
#include <game_math.h>
#include <string>
#include "eComponentType.h"

namespace nPhysics
{
	class iPhysicsComponent
	{
	public:
		virtual ~iPhysicsComponent() {}
		inline const eComponentType& GetComponentType() { return mComponentType; }

		virtual void GetTransform(glm::mat4& transformOut) = 0;
		virtual void ApplyForce(glm::vec3 force) = 0;
		virtual std::string ToString() = 0;
		virtual void ApplyImpulse(glm::vec3 impulse) = 0;

	protected:
		iPhysicsComponent(eComponentType componentType)
			: mComponentType(componentType) {}
	private:
		eComponentType mComponentType;

		// get rid of these!
		iPhysicsComponent() = delete;
		iPhysicsComponent(const iPhysicsComponent& other) = delete;
		iPhysicsComponent& operator=(const iPhysicsComponent& other) = delete;
	};
}