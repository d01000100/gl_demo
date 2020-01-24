#include "Steerings.h"
#include "util.h"
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "globalStuff.h"
#include "colors.h"

glm::vec3 Steerings::seek(glm::vec3 pos, glm::vec3 vel, glm::vec3 target, float maxVel)
{
	if (glm::distance(pos, target) > 0)
	{
		glm::vec3 desiredVelocity = glm::normalize(target - pos) * maxVel;
		glm::vec3 steeringVel = desiredVelocity - vel;
		return steeringVel;
	}
	return glm::vec3(0);
}

glm::vec3 Steerings::flee(glm::vec3 pos, glm::vec3 vel, glm::vec3 target, float maxVel)
{
	return seek(target, vel, pos, maxVel);
}

glm::vec3 Steerings::approach(glm::vec3 pos, glm::vec3 vel, glm::vec3 target, float maxVel, float slowingRadius, float stopRadius)
{
	glm::vec3 desiredVelocity = glm::normalize(target - pos) * maxVel;
	float distance = glm::distance(pos, target);
	if (distance < stopRadius)
	{
		//printf("Stop!!!\n");
		return glm::vec3(0);
	}
	else if (distance < slowingRadius)
	{
		desiredVelocity = glm::normalize(desiredVelocity) * maxVel * 
			(distance - stopRadius) / (slowingRadius - stopRadius);
		//printf("Slowing down... %f\n", distance / slowingRadius);
	}
	glm::vec3 steeringVel = desiredVelocity - vel;
	return steeringVel;
}

glm::vec3 Steerings::wander(
	glm::vec3 pos, 
	glm::vec3 vel,
	float maxVel,
	glm::vec3 direction,
	float distanceToPoint,
	float circleRadius)
{
	// get the circle in world space
	glm::vec3 circleCenter = pos + direction * distanceToPoint;
	// get a random angle 
	double angle = glm::radians(randInRange(-180, 180));
	// calculate circle coordinates
	double x = glm::cos(angle) * circleRadius,
		z = glm::sin(angle) * circleRadius;
	glm::vec3 seekPoint = circleCenter + glm::vec3(x, 0, z);
	// seek towards that
	return seek(pos, vel, seekPoint, maxVel);
}

glm::vec3 Steerings::pursue(
	glm::vec3 pos, 
	glm::vec3 vel, 
	glm::vec3 target,
	glm::vec3 targetVel,
	float maxVel)
{
	float distance = glm::distance(pos, target);
	float framesToPredict = distance / maxVel;
	std::cout << "Distance: " << distance << " frames: "<< framesToPredict << std::endl;
	glm::vec3 futureTarget = target + targetVel * framesToPredict;
	g_pDebugRenderer->addLine(target, futureTarget, Colors::white, 0.01f);
	return seek(pos, vel, futureTarget, maxVel);
}

glm::vec3 Steerings::evade(
	glm::vec3 pos,
	glm::vec3 vel,
	glm::vec3 target,
	glm::vec3 targetVel,
	float maxVel)
{
	float distance = glm::distance(pos, target);
	float framesToPredict = distance / maxVel;
	//std::cout << "Distance: " << distance << " frames: " << framesToPredict << std::endl;
	glm::vec3 futureTarget = target + targetVel * framesToPredict;
	g_pDebugRenderer->addLine(target, futureTarget, Colors::white, 0.01f);
	return flee(pos, vel, futureTarget, maxVel);
}