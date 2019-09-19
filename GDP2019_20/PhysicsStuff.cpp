// PhysicsStuff.cpp
#include <vector>
#include "cGameObject.h"
#include <iostream>

void PhysicsUpdate(std::vector<cGameObject>& vecGameObjects,
				   float deltaTime)
{

	// 9.81 
	glm::vec3 gravity = glm::vec3( 0.0f, -6.0f, 0.0f );


	for (unsigned int index = 0;
		 index != vecGameObjects.size(); index++)
	{


		cGameObject& CurObj = vecGameObjects[index];

		if (CurObj.inverseMass != 0.0f)
		{

			// Forward Explicit Euler Inetegration
			//NewVelocty += Velocity + ( Ax * DeltaTime )

			// 
			CurObj.accel = gravity;


			CurObj.velocity.x += CurObj.accel.x * deltaTime;
			CurObj.velocity.y += CurObj.accel.y * deltaTime;
			CurObj.velocity.z += CurObj.accel.z * deltaTime;
	//		// Or you can do this...
	//		CurObj.velocity += CurObj.accel * deltaTime;

			//NewPosition = Posistion + ( Vx * DeltaTime )

			CurObj.positionXYZ.x += CurObj.velocity.x * deltaTime;
			CurObj.positionXYZ.y += CurObj.velocity.y * deltaTime;
			CurObj.positionXYZ.z += CurObj.velocity.z * deltaTime;


			// Test to see if it's hit the cube
			if ( (CurObj.positionXYZ.y - 1.0f) <= 0.0f)
			{
				// It's hit the cube. Set the velocity to -ve of what it is
				// fabs() is floating point absolute value
				CurObj.velocity.y = fabs(CurObj.velocity.y);

				// Damp the velocity 
				CurObj.velocity.y *= 0.95f;
			}

			// Left plane 
			if ( (CurObj.positionXYZ.x) <= -10.0f)
			{
				// It's hit the cube. Set the velocity to -ve of what it is
				// fabs() is floating point absolute value
				CurObj.velocity.x = +fabs(CurObj.velocity.x);
			}
			// Right plane 
			if ( (CurObj.positionXYZ.x) >= +10.0f)
			{
				// It's hit the cube. Set the velocity to -ve of what it is
				// fabs() is floating point absolute value
				CurObj.velocity.x = -fabs(CurObj.velocity.x);
			}

		}// if (CurObj.inverseMass != 0.0f)

	}

	return;
}



// Amazing notes
	// Determine the object new position

	// Position   ( x = 0 )

	// Velocity   Vx = 1.0f / second 

	//Where is the object at time = 1? 

	// DeltaX  = Vx * DeltaTime 
	//    1.0     =  1.0f/second  * 1.0 second

//Position += (DeltaX)
//Position += Vx * DeltaTime

// Velocity = 0.0f / second 
// Accel = 1.0f / Second /second 

// DeltaV  = Ax * DeltaTime 
// DeltaV  = 1.0f/second/second * 1.0f second 
// DeltaV  = 1.0f/second/ * 1.0f  
// Velocty += DeltaV
// Velocty += Ax * DeltaTime 

