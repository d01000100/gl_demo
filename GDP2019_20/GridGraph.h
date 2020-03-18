#pragma once
#include <glm/vec2.hpp>
#include <vector>

typedef std::vector<glm::vec2> vCoords;

struct GridGraph
{
	static bool isValid(glm::vec2 coord);
	static vCoords getNeighbors(glm::vec2 coord);
	static float manhattanDistance(glm::vec2 a, glm::vec2 b);
	static float getEdgeWeight(glm::vec2 origin, glm::vec2 destination);
	static void printCoords(vCoords coords);
};

