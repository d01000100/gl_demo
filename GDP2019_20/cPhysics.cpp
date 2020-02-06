#include "cPhysics.h"
#include "globalStuff.h"

cPhysics::cPhysics()
{
	this->m_Gravity = glm::vec3(0.0f, -1.0f, 0.0f);
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

void cPhysics::IntegrationStep(std::vector<cGameObject*> vec_pGameObjects, float deltaTime)
{
	for (unsigned int index = 0;
		 index != vec_pGameObjects.size(); index++)
	{
		// Get a pointer to the current object (makes the code a little clearer)
		cGameObject* pCurObj = vec_pGameObjects[index];

		if (pCurObj->physics)
		{
			sPhysicsObject* phObj = pCurObj->physics;
			
			if (phObj->gravity) {
				phObj->acceleration += m_Gravity;
			}

			// Forward Explicit Euler Inetegration
			//NewVelocty += Velocity + ( Ax * DeltaTime )
			phObj->velocity += phObj->acceleration * deltaTime;
			pCurObj->position += phObj->velocity * deltaTime;
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
		sPlyVertex triVert1 = mesh.vecVertices[curTriangle.vert_index_1];
		sPlyVertex triVert2 = mesh.vecVertices[curTriangle.vert_index_2];
		sPlyVertex triVert3 = mesh.vecVertices[curTriangle.vert_index_3];

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
			closestTriangle.verts[0].x = triVert1.x;
			closestTriangle.verts[0].y = triVert1.y;
			closestTriangle.verts[0].z = triVert1.z;
			closestTriangle.verts[1].x = triVert2.x;
			closestTriangle.verts[1].y = triVert2.y;
			closestTriangle.verts[1].z = triVert2.z;
			closestTriangle.verts[2].x = triVert3.x;
			closestTriangle.verts[2].y = triVert3.y;
			closestTriangle.verts[2].z = triVert3.z;
			
			// Quick is to average the normal of all 3 vertices
			glm::vec3 triVert1Norm = glm::vec3(triVert1.nx, triVert1.ny, triVert1.nz );
			glm::vec3 triVert2Norm = glm::vec3(triVert2.nx, triVert2.ny, triVert2.nz );
			glm::vec3 triVert3Norm = glm::vec3(triVert3.nx, triVert3.ny, triVert3.nz );

			// Average of the vertex normals... 
			closestTriangle.normal = (triVert1Norm + triVert2Norm + triVert3Norm) / 3.0f;

		}

	}//for (unsigned int triIndex = 0;

	return;
}

// Will return the closest triangles that are within the range "distanceRange".
// This can be used as a "closest triangles to sphere"
void cPhysics::GetClosestTrianglesToSphere(cGameObject& testSphere, float distanceRange, cMesh& mesh, std::vector<sPhysicsTriangle>& vecClosestTriangles)
{



}

// Test each object with every other object
void cPhysics::TestForCollisions(std::vector<cGameObject*> vec_pGameObjects)
{
	// This will store all the collisions in this frame
	std::vector<sCollisionInfo> vecCollisions;

	sCollisionInfo collisionInfo;

	for (unsigned int outerLoopIndex = 0;
		 outerLoopIndex != vec_pGameObjects.size(); outerLoopIndex++)
	{
		for (unsigned int innerLoopIndex = 0;
			 innerLoopIndex != vec_pGameObjects.size(); innerLoopIndex++)
		{ 
			cGameObject* pA = vec_pGameObjects[outerLoopIndex];
			cGameObject* pB = vec_pGameObjects[innerLoopIndex];

			// Am I testing the object with itself? 
			if ( pA->getUniqueID() == pB->getUniqueID() )
			{	
				continue;
			}
			else if (pA->physics && pB->physics) {
				if (pA->physics->shape == SPHERE &&
					pB->physics->shape == SPHERE)
				{
					if (DoSphereSphereCollision(pA, pB, collisionInfo))
					{
						vecCollisions.push_back(collisionInfo);
					}
				}
				else if (pA->physics->shape == SPHERE &&
						 pB->physics->shape == MESH)
				{
					if (DoSphereMeshCollision(pA, pB, collisionInfo))
					{
						vecCollisions.push_back(collisionInfo);
					}
				}		
			}
		}//for (unsigned int innerLoopIndex = 0;
	}//for (unsigned int outerLoopIndex = 0;
}

bool cPhysics::DoSphereSphereCollision(cGameObject* pA, cGameObject* pB,
								 sCollisionInfo& collisionInfo)
{
	// B -> A (en origen)
	glm::vec3 distance = pA->position - pB->position;
	float clippingDist = pA->physics->radius + pB->physics->radius - glm::length(distance);
	if (clippingDist > 0) {

		glm::vec3 normDist = glm::normalize(distance);

		pA->position += clippingDist * normDist;

		// reflect ball A according to the vector between the balls
		glm::vec3 vel_reflected = glm::reflect(pA->physics->velocity, normDist);
		vel_reflected = glm::normalize(vel_reflected);

		pA->physics->velocity = vel_reflected * glm::length(pA->physics->velocity);

		// reflect ball B according to the inverseo of the vector between the balls
		vel_reflected = glm::reflect(pB->physics->velocity, -normDist);
		vel_reflected = glm::normalize(vel_reflected);

		pB->physics->velocity = vel_reflected * glm::length(pB->physics->velocity);

		//pB->physics->acceleration = glm::vec3(0.0f);
		//pB->physics->velocity = glm::vec3(0.0f);
		//pB->physics->gravity = false;

		//pA->physics->acceleration = glm::vec3(0.0f);
		//pA->physics->velocity = glm::vec3(0.0f);
		//pA->physics->gravity = false;

		return true;
	}

	return false;
}
bool cPhysics::DoSphereMeshCollision(cGameObject* sphere, cGameObject* mesh,
								sCollisionInfo& collisionInfo)
{
	glm::vec3 closestPoint;
	sPhysicsTriangle closestTriangle;

	// Get a transformed mesh to match the object's position and rotation
	cMesh transformed_mesh =  transformMesh(
		*mesh->physics->mesh, 
		mesh->calculateTransformationMatrix());

	GetClosestTriangleToPoint(sphere->position, transformed_mesh, closestPoint, closestTriangle);

	float distance = glm::length(sphere->position - closestPoint);

	// Are we hitting the triangle? 
	if (distance <= sphere->physics->radius)
	{
		// Move the sphere back to where it just penetrated...
		// So that it will collide exactly where it's supposed to. 

		// 1. Calculate vector from centre of sphere to closest point
		glm::vec3 vecSphereToClosestPoint = closestPoint - sphere->position;

		// 2. Get the length of this vector
		float centreToContractDistance = glm::length(vecSphereToClosestPoint);

		// 3. Create a vector from closest point to radius
		float lengthPositionAdjustment = sphere->physics->radius - centreToContractDistance;

		// 4. Sphere is moving in the direction of the velocity, so 
		//    we want to move the sphere BACK along this velocity vector
		glm::vec3 vecDirection = glm::normalize(sphere->physics->velocity);

		glm::vec3 vecPositionAdjust = (-vecDirection) * lengthPositionAdjustment;

		// 5. Reposition sphere 
		sphere->position += (vecPositionAdjust);

		// Is in contact with the triangle... 
		
		// draw velocity
		debugRenderer->addLine(closestPoint, closestPoint - sphere->physics->velocity * 0.2f, glm::vec3(1.0f, 0.0f, 0.0f), 500.0f);
		// draw normal
		debugRenderer->addLine(closestPoint, closestPoint + closestTriangle.normal * 10.0f, glm::vec3(1.0f, 1.0f, 1.0f), 500.0f);

		// Calculate the response vector off the triangle. 
		glm::vec3 velocityVector = glm::normalize(sphere->physics->velocity);


		// closestTriangle.normal
		glm::vec3 reflectionVec = glm::reflect(velocityVector, glm::normalize(closestTriangle.normal));
		// draw reflection

		reflectionVec = glm::normalize(reflectionVec);

		// Change the direction of the ball (the bounce off the triangle)

		// Get lenght of the velocity vector
		float speed = glm::length(sphere->physics->velocity);

		sphere->physics->velocity = reflectionVec * speed;
		debugRenderer->addLine(closestPoint, closestPoint + sphere->physics->velocity * 0.2f, glm::vec3(0.0f, 0.0f, 1.0f), 500.0f);
		//sphere->physics = NULL;
		return true;
	}
	return false;
}

cMesh cPhysics::transformMesh(cMesh mesh, glm::mat4 tMat) {
	// Using the same thing that happens in the shader, 
	// we transform the vertices of the mesh by the world matrix

	for (std::vector<sPlyVertex>::iterator itVert = mesh.vecVertices.begin();
		itVert != mesh.vecVertices.end(); itVert++)
	{
		glm::vec4 vertex = glm::vec4(itVert->x, itVert->y, itVert->z, 1.0f);

		glm::vec4 vertexWorldTransformed = tMat * vertex;

		// Update 
		// itVert is not a vec3, so we need to do this manyally
		itVert->x = vertexWorldTransformed.x;
		itVert->y = vertexWorldTransformed.y;
		itVert->z = vertexWorldTransformed.z;


		// CALCAULTE THE NORMALS for the this mesh, too (for the response)
		// for the normal, do the inverse transpose of the world matrix
		glm::mat4 matWorld_Inv_Transp = glm::inverse(glm::transpose(tMat));
		glm::vec4 normal = glm::vec4(itVert->nx, itVert->ny, itVert->nz, 1.0f);
		glm::vec4 normalWorldTransformed = matWorld_Inv_Transp * normal;

		// Update 
		itVert->nx = normalWorldTransformed.x;
		itVert->ny = normalWorldTransformed.y;
		itVert->nz = normalWorldTransformed.z;
	}

	return mesh;
}