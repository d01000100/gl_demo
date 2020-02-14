#include "Coordinator.h"
#include "Steerings.h"
#include "Scene.h"

float Coordinator::formationDistance = 12.0f;

std::vector<glm::vec3> Coordinator::line = {
	// 5 left
	glm::vec3(-1, 0, 0) * (1 * Coordinator::formationDistance),
	glm::vec3(-1, 0, 0) * (2 * Coordinator::formationDistance),
	glm::vec3(-1, 0, 0) * (3 * Coordinator::formationDistance),
	glm::vec3(-1, 0, 0) * (4 * Coordinator::formationDistance),
	glm::vec3(-1, 0, 0) * (5 * Coordinator::formationDistance),
	// The leader is at local 0,0,0
	// 6 right
	glm::vec3(1, 0, 0)* (1 * Coordinator::formationDistance),
	glm::vec3(1, 0, 0)* (2 * Coordinator::formationDistance),
	glm::vec3(1, 0, 0)* (3 * Coordinator::formationDistance),
	glm::vec3(1, 0, 0)* (4 * Coordinator::formationDistance),
	glm::vec3(1, 0, 0)* (5 * Coordinator::formationDistance),
	glm::vec3(1, 0, 0)* (6 * Coordinator::formationDistance)
};

std::vector<glm::vec3> Coordinator::vFormation{
	// Leader is on the front
	// Left arm is 6 vehicles long
	glm::vec3(-1, 0, -1) * (1 * Coordinator::formationDistance),
	glm::vec3(-1, 0, -1) * (2 * Coordinator::formationDistance),
	glm::vec3(-1, 0, -1) * (3 * Coordinator::formationDistance),
	glm::vec3(-1, 0, -1) * (4 * Coordinator::formationDistance),
	glm::vec3(-1, 0, -1) * (5 * Coordinator::formationDistance),
	glm::vec3(-1, 0, -1) * (6 * Coordinator::formationDistance),
	// Right arm is 5 vehicles long
	glm::vec3(1, 0, -1)* (1 * Coordinator::formationDistance),
	glm::vec3(1, 0, -1)* (2 * Coordinator::formationDistance),
	glm::vec3(1, 0, -1)* (3 * Coordinator::formationDistance),
	glm::vec3(1, 0, -1)* (4 * Coordinator::formationDistance),
	glm::vec3(1, 0, -1)* (5 * Coordinator::formationDistance)
};

std::vector<glm::vec3> Coordinator::twoLines{
	// Leader is in front of the left line
	// Left line is 5 vehicles long behind the leader
	glm::vec3(0, 0, -1) * (1 * Coordinator::formationDistance),
	glm::vec3(0, 0, -1) * (2 * Coordinator::formationDistance),
	glm::vec3(0, 0, -1) * (3 * Coordinator::formationDistance),
	glm::vec3(0, 0, -1) * (4 * Coordinator::formationDistance),
	glm::vec3(0, 0, -1) * (5 * Coordinator::formationDistance),
	// right line is 6 vehicles long.
	// Starts right of the leader
	glm::vec3(1,0,0) * (2 * Coordinator::formationDistance),
	// And goes 5 more behind that vehicle
	glm::vec3(1,0,0) * (2 * Coordinator::formationDistance) + glm::vec3(0, 0, -1) * (1 * Coordinator::formationDistance),
	glm::vec3(1,0,0) * (2 * Coordinator::formationDistance) + glm::vec3(0, 0, -1) * (2 * Coordinator::formationDistance),
	glm::vec3(1,0,0) * (2 * Coordinator::formationDistance) + glm::vec3(0, 0, -1) * (3 * Coordinator::formationDistance),
	glm::vec3(1,0,0) * (2 * Coordinator::formationDistance) + glm::vec3(0, 0, -1) * (4 * Coordinator::formationDistance),
	glm::vec3(1,0,0) * (2 * Coordinator::formationDistance) + glm::vec3(0, 0, -1) * (5 * Coordinator::formationDistance)
};

std::vector<glm::vec3> Coordinator::square{
	// Leader is on front of the square, somewhat left
	// Theres one left of the leader
	glm::vec3(-1,0,0) * formationDistance,
	// Theres 3 behind that one
	glm::vec3(-1,0,0)* formationDistance + glm::vec3(0,0,-1) * (1 * Coordinator::formationDistance),
	glm::vec3(-1,0,0)* formationDistance + glm::vec3(0,0,-1) * (2 * Coordinator::formationDistance),
	glm::vec3(-1,0,0)* formationDistance + glm::vec3(0,0,-1) * (3 * Coordinator::formationDistance),
	// Theres 2 right of the leader
	glm::vec3(1,0,0)* (1 * formationDistance),
	glm::vec3(1,0,0)* (2 * formationDistance),
	// There are 3 more on the right side of the square
	glm::vec3(1,0,0)* (2 * formationDistance) + glm::vec3(0,0,-1) * (1 * Coordinator::formationDistance),
	glm::vec3(1,0,0)* (2 * formationDistance) + glm::vec3(0,0,-1) * (2 * Coordinator::formationDistance),
	glm::vec3(1,0,0)* (2 * formationDistance) + glm::vec3(0,0,-1) * (3 * Coordinator::formationDistance),
	// Theres one 3 places behind the leader
	glm::vec3(0, 0, -1)* (3 * Coordinator::formationDistance),
	// Theres one to the right of that one
	glm::vec3(0, 0, -1) * (3 * Coordinator::formationDistance) + glm::vec3(1,0,0) * Coordinator::formationDistance
};

std::vector<glm::vec3> Coordinator::circle = {
	// the leader is in front of the circle
	// left side of the circle
	glm::vec3(4.599999904632568, 0.0, -1.8000001907348633) * 3.f,
	glm::vec3(8.599996566772461, 0.0, -5.799998760223389 ) * 3.f,
	glm::vec3(11.199994087219238, 0.0, -10.59999465942382) * 3.f,
	glm::vec3(9.199995994567871, 0.0, -15.199990272521973) * 3.f,
	glm::vec3(5.1999993324279785, 0.0, -19.0) * 3.f,
	// behind of the circle
	glm::vec3(0.0, 0, -21) * 3.f,
	// right side of the cirle
	glm::vec3(-4.599999904632568, 0.0, -1.8000001907348633) * 3.f,
	glm::vec3(-8.599996566772461, 0.0, -5.799998760223389) * 3.f,
	glm::vec3(-11.199994087219238, 0.0, -10.59999465942382) * 3.f,
	glm::vec3(-9.199995994567871, 0.0, -15.199990272521973) * 3.f,
	glm::vec3(-5.1999993324279785, 0.0, -19.0) * 3.f
};

Coordinator::Coordinator(cGameObject* l, std::vector<glm::vec3> formation) :
	leader(l)
{
	changeFormation(formation);	
}

void Coordinator::changeFormation(std::vector<glm::vec3> formation)
{
	leader->collision_points = formation;
}

void Coordinator::update(float deltaTime)
{
	std::vector<glm::vec3> seekPoints = leader->getTransformedCollisionPoints();
	// It maps a position with each one of the members of the batallion
	// in order.
	for (int i = 0; i < seekPoints.size() && i < batallion.size(); ++i)
	{
		glm::vec3 seekPoint = seekPoints[i];
		cGameObject* vehicle = batallion[i];

		float distance = glm::distance(seekPoint, vehicle->getPos());
		float maxVel = distance * 10.0f;
		glm::vec3 steeringVel = Steerings::seek(
			vehicle->getPos(),
			vehicle->physics->velocity,
			seekPoint,
			maxVel);
		
		if (distance < 0.1f)
		{
			vehicle->physics->velocity = glm::vec3(0);
		}
		else
		{
			vehicle->physics->velocity += steeringVel * deltaTime * 3.0f;
			if (glm::length(vehicle->physics->velocity) > maxVel)
				vehicle->physics->velocity = glm::normalize(vehicle->physics->velocity) * maxVel;
		}
		vehicle->setDirection(vehicle->physics->velocity);
	}
}
