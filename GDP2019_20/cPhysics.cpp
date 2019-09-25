#include "cPhysics.h"

cPhysics::cPhysics()
{
	// This is a typical Earth gravity value. 
	// note that this doesn't mean that the simulation will "look good", though... 
	this->m_Gravity = glm::vec3(0.0f, -9.81f, 0.0f);
	return;
}


void cPhysics::setGravity(glm::vec3 newGravityValue)
{
	this->m_Gravity = newGravityValue;
	return;
}

glm::vec3 cPhysics::getGravity(void)
{
	return this->m_Gravity;
}


void cPhysics::IntegrationStep(std::vector<cGameObject*>& vec_pGameObjects, float deltaTime)
{


	for (unsigned int index = 0;
		 index != vec_pGameObjects.size(); index++)
	{
		// Get a pointer to the current object (makes the code a little clearer)
		cGameObject* pCurObj = vec_pGameObjects[index];

		if (pCurObj->inverseMass != 0.0f)
		{

			// Forward Explicit Euler Inetegration
			//NewVelocty += Velocity + ( Ax * DeltaTime )

			// 
			pCurObj->accel = this->m_Gravity;


			pCurObj->velocity.x += pCurObj->accel.x * deltaTime;
			pCurObj->velocity.y += pCurObj->accel.y * deltaTime;
			pCurObj->velocity.z += pCurObj->accel.z * deltaTime;
			//		// Or you can do this...
			//		CurObj.velocity += CurObj.accel * deltaTime;

					//NewPosition = Posistion + ( Vx * DeltaTime )

			pCurObj->positionXYZ.x += pCurObj->velocity.x * deltaTime;
			pCurObj->positionXYZ.y += pCurObj->velocity.y * deltaTime;
			pCurObj->positionXYZ.z += pCurObj->velocity.z * deltaTime;


		}
	}//for (unsigned int index = 0;

	return;
}



// Returns all the triangles and the closest points
void cPhysics::GetClosestTriangleToPoint(Point pointXYZ, cMesh& mesh, glm::vec3& closestPoint, sPhysicsTriangle& closestTriangle)
{

	// Assume the closest distance is REALLY far away
	float closestDistanceSoFar = FLT_MAX;


	for (unsigned int triIndex = 0;
		 triIndex != mesh.vecTriangles.size();
		 triIndex++)
	{
		sPlyTriangle& curTriangle = mesh.vecTriangles[triIndex];

		// Get the vertices of the triangle
		sPlyVertexXYZ_N triVert1 = mesh.vecVertices[curTriangle.vert_index_1];
		sPlyVertexXYZ_N triVert2 = mesh.vecVertices[curTriangle.vert_index_2];
		sPlyVertexXYZ_N triVert3 = mesh.vecVertices[curTriangle.vert_index_3];

		Point triVertPoint1;
		triVertPoint1.x = triVert1.x;
		triVertPoint1.y = triVert1.y;
		triVertPoint1.z = triVert1.z;

		Point triVertPoint2;
		triVertPoint2.x = triVert2.x;
		triVertPoint2.y = triVert2.y;
		triVertPoint2.z = triVert2.z;

		Point triVertPoint3;
		triVertPoint3.x = triVert3.x;
		triVertPoint3.y = triVert3.y;
		triVertPoint3.z = triVert3.z;

		glm::vec3 curClosetPoint = ClosestPtPointTriangle( pointXYZ,
														   triVertPoint1, triVertPoint2, triVertPoint3);

		// Is this the closest so far?
		float distanceNow = glm::distance(curClosetPoint, pointXYZ);

		// is this closer than the closest distance
		if (distanceNow <= closestDistanceSoFar)
		{
			closestDistanceSoFar = distanceNow;

			closestPoint = curClosetPoint;

			// Copy the triangle information over, as well
			closestTriangle.verts[0].x = mesh.vecVertices[mesh.vecTriangles[0].vert_index_1].x;
			closestTriangle.verts[0].y = mesh.vecVertices[mesh.vecTriangles[0].vert_index_1].y;
			closestTriangle.verts[0].z = mesh.vecVertices[mesh.vecTriangles[0].vert_index_1].z;
			closestTriangle.verts[1].x = mesh.vecVertices[mesh.vecTriangles[0].vert_index_2].x;
			closestTriangle.verts[1].y = mesh.vecVertices[mesh.vecTriangles[0].vert_index_2].y;
			closestTriangle.verts[1].z = mesh.vecVertices[mesh.vecTriangles[0].vert_index_2].z;
			closestTriangle.verts[2].x = mesh.vecVertices[mesh.vecTriangles[0].vert_index_3].x;
			closestTriangle.verts[2].y = mesh.vecVertices[mesh.vecTriangles[0].vert_index_3].y;
			closestTriangle.verts[2].z = mesh.vecVertices[mesh.vecTriangles[0].vert_index_3].z;
			
			// TODO: Copy the normal, too	

		}

	}//for (unsigned int triIndex = 0;

	return;
}

// Will return the closest triangles that are within the range "distanceRange".
// This can be used as a "closest triangles to sphere"
void cPhysics::GetClosestTrianglesToSphere(cGameObject& testSphere, float distanceRange, cMesh& mesh, std::vector<sPhysicsTriangle>& vecClosestTriangles)
{



}
