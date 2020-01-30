#include "Util.h"

std::string phys::Util::RigidBodyToString(const cRigidBody& body)
{
	std::string res;
	switch(body.GetShapeType())
	{
	case eShapeType::sphere:
		res += "Sphere: ";
		break;
	case eShapeType::plane:
		res += "Plane: ";
		break;
	default:
		res += "Unrecognized shape: ";
	}
	res += " pos: " + glm::to_string(body.mPosition);
	res += " vel: " + glm::to_string(body.mVelocity);
	return res;
}

void phys::Util::PrintRigidBody(const cRigidBody& body)
{
	printf("%s\n", RigidBodyToString(body).c_str());
}
