#include "BroadCollision.h"
#include "globalStuff.h"
#include "colors.h"
#include "Scene.h"

#include <glm/gtx/projection.hpp>

cPhysics BroadCollision::physicsStuff;
float BroadCollision::collisionPointSize = 1.0f;

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
					//::g_pDebugRenderer->addPoint(collisionPoint, Colors::red, 3.0f, 3.0f);
					::g_pDebugRenderer->addTriangle((*itT)->a, (*itT)->b, (*itT)->c, Colors::blue, 3.0f);
					cGameObject* explosion = new cGameObject();
					explosion->meshName = "sphere_model";
					explosion->friendlyName = explosion->meshName + " " + std::to_string(explosion->getUniqueID());
					explosion->diffuseColor = Colors::red;
					explosion->alpha = 0.7f;
					explosion->scale = 5.0f;
					explosion->lifeTime = 3.0f;
					explosion->position = pointInTriangle;

					Scene::getTheScene()->addItem(explosion);

					sCollisionInfo collisionInfo;

					glm::vec3 collisionDir = pointInTriangle - collisionPoint;

					collisionInfo.closestPoint = pointInTriangle;
					collisionInfo.directionOfApproach = glm::normalize(pointInTriangle - collisionPoint);
					collisionInfo.penetrationDistance = collisionPointSize - distanceToTriangle;
					collisionInfo.adjustmentVector = 
						collisionInfo.penetrationDistance *
						-collisionInfo.directionOfApproach;

					glm::vec3 velocity = glm::normalize(mobile->physics->velocity);
					float speed = glm::length(mobile->physics->velocity);
					collisionInfo.normal = (*itT)->normal;

					res.push_back(collisionInfo);
				}
			}
		}
	}

	return res;
}

void BroadCollision::collisionsReact(std::vector<sCollisionInfo> collisions, cGameObject* mobile)
{
	if (!collisions.empty()) 
	{
		glm::vec3 reflection(0);
		for (int c = 0; c < collisions.size(); c++)
		{
			mobile->position += collisions[c].adjustmentVector;
			//mobile->physics->velocity = glm::proj(mobile->physics->velocity, collisions[c].normal);
		}

		//reflection /= collisions.size();
		//sMessage toObject; toObject.name = "stop";
		//mobile->recieveMessage(toObject);
		//mobile->physics->velocity = reflection;
	}
	return;
}