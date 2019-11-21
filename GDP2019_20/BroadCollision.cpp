#include "BroadCollision.h"
#include "globalStuff.h"
#include "colors.h"

cPhysics BroadCollision::physicsStuff;
float BroadCollision::collisionPointSize = 0.1f;

std::vector<sCollisionInfo> BroadCollision::detectCollisions(AABBGrid* grid, cGameObject* mobile)
{
	std::vector<sCollisionInfo> res;

	std::vector<glm::vec3> collisionPoints = mobile->getTransformedCollisionPoints();

	for (int p = 0; p < collisionPoints.size(); p++)
	{
		glm::vec3 collisionPoint = collisionPoints.at(p);

		cAABB* pAABB = grid->findAABB(collisionPoint);

		if (pAABB)
		{
			pAABB->Draw();
			for (std::set<sNiceTriangle*>::iterator itT = pAABB->triangles.begin();
				itT != pAABB->triangles.end(); itT++)
			{
				glm::vec3 pointInTriangle = physicsStuff.ClosestPtPointTriangle(
					collisionPoint,
					(*itT)->a,
					(*itT)->b,
					(*itT)->c);


				float distanceToTriangle = distance(pointInTriangle, collisionPoint);

				// TODO: Check also if we're on the "outside" of the mesh
				if (distanceToTriangle <= collisionPointSize)
				{
					::g_pDebugRenderer->addPoint(collisionPoint, Colors::red, 3.0f, 3.0f);
					::g_pDebugRenderer->addTriangle((*itT)->a, (*itT)->b, (*itT)->c, Colors::blue, 3.0f);
					sCollisionInfo collisionInfo;

					glm::vec3 collisionDir = pointInTriangle - collisionPoint;

					collisionInfo.closestPoint = pointInTriangle;
					collisionInfo.directionOfApproach = glm::normalize(pointInTriangle - collisionPoint);
					collisionInfo.penetrationDistance = collisionPointSize - distanceToTriangle;

					res.push_back(collisionInfo);
				}
			}
		}
	}

	return res;
}

void BroadCollision::collisionsReact(std::vector<sCollisionInfo>, cGameObject* mobile)
{
	return;
}