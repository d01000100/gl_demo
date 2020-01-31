#include <iostream>
#include "wrapper/myPhysics/cPhysicsFactory.h"

using namespace nPhysics;

int main()
{
	cPhysicsFactory factory;
	sBallDef ballDef;
	ballDef.Position = glm::vec3(0,10,0);
	iPhysicsComponent *ball = factory.CreateBall(ballDef); 

	sPlaneDef planeDef;
	planeDef.Normal = glm::vec3(0, 1, 0);
	planeDef.Point = glm::vec3(5);
	iPhysicsComponent* plane = factory.CreatePlane(planeDef);

	iPhysicsWorld *theWorld = factory.CreateWorld();
	theWorld->AddComponent(ball);
	theWorld->AddComponent(plane);

	float deltaTime = 0.1f;

	for (int i = 0;i < 40;i++)
	{
		//sphereBody.ApplyForce(glm::vec3(0,0,1));
		theWorld->Update(deltaTime);
		std::cout << "Sphere 1: ";
		std::cout << ball->ToString() << std::endl;
		//std::cout << "Sphere 2: ";
		//phys::Util::PrintRigidBody(sphere2Body);
		printf("\n");
	}
	
	theWorld->RemoveComponent(ball);
	theWorld->RemoveComponent(plane);

	return 0;
}
