#include "phys/cWorld.h"
#include "phys/Util.h"
#include <iostream>

int main()
{
	std::cout << "Testing my beautiful physics library!!!\n";

	phys::sRigidBodyDef sphereBodyDef;
	sphereBodyDef.Position = glm::vec3(0,10,0);
	sphereBodyDef.Velocity = glm::vec3(0, -10, 0);
	phys::cSphere sphereShape(1.0f);
	phys::cRigidBody sphereBody(sphereBodyDef, &sphereShape);

	phys::sRigidBodyDef sphereBody2Def;
	sphereBody2Def.Position = glm::vec3(0,-10,0);
	sphereBody2Def.Velocity = glm::vec3(1, 10, 0);
	sphereBody2Def.Mass = 3.0f;
	phys::cSphere sphere2Shape(1.0f);
	phys::cRigidBody sphere2Body(sphereBody2Def, &sphere2Shape);

	phys::sRigidBodyDef planeBodyDef;
	planeBodyDef.Mass = 0.0f;
	phys::cPlane planeShape(glm::vec3(0,1,0),0);
	phys::cRigidBody planeBody(planeBodyDef,&planeShape);

	phys::cWorld physWorld;
	physWorld.AddRigidBody(&sphereBody);
	physWorld.AddRigidBody(&sphere2Body);
	//physWorld.AddRigidBody(&planeBody);
	physWorld.SetGravity(glm::vec3(0,0,0));

	float deltaTime = 0.1f;

	for (int i = 0;i < 40;i++)
	{
		//sphereBody.ApplyForce(glm::vec3(0,0,1));
		physWorld.Update(deltaTime);
		std::cout << "Sphere 1: ";
		phys::Util::PrintRigidBody(sphereBody);
		std::cout << "Sphere 2: ";
		phys::Util::PrintRigidBody(sphere2Body);
		printf("\n");
	}
		
	return 0;
}
