#include <cstdio>
#include "phys/cWorld.h"
#include "phys/Util.h"

int main()
{
	printf("Testing my beautiful physics library!!!\n");

	phys::sRigidBodyDef sphereBodyDef;
	sphereBodyDef.Position = glm::vec3(0,20,0);
	sphereBodyDef.Velocity = glm::vec3(10, 10, 0);
	phys::cSphere sphereShape(3.0f);
	phys::cRigidBody sphereBody(sphereBodyDef, &sphereShape);

	phys::cWorld physWorld;
	physWorld.AddRigidBody(&sphereBody);
	physWorld.SetGravity(glm::vec3(0,-10,0));

	float deltaTime = 0.1f;

	for (int i = 0;i < 20;i++)
	{
		sphereBody.ApplyForce(glm::vec3(0,0,1));
		physWorld.Update(deltaTime);
		phys::Util::PrintRigidBody(sphereBody);
	}
		
	return 0;
}
