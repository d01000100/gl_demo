#pragma once
#include "cGameObject.h"

struct Coordinator
{
	cGameObject* leader;
	static float formationDistance;
	static std::vector<glm::vec3> circle, vFormation, square, line, twoLines;
	std::vector<cGameObject*> batallion;

	Coordinator(cGameObject* l, std::vector<glm::vec3> formation = Coordinator::circle);
	void changeFormation(std::vector<glm::vec3> formation);
	void update(float deltaTime);
};

