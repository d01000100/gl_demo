#pragma once
#include <string>
#include "cRigidBody.h"

namespace phys
{
	class Util
	{
	public:
		static std::string RigidBodyToString(const cRigidBody& body);
		static void PrintRigidBody(const cRigidBody& body);
	};
}

