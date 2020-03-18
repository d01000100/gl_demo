#pragma once
#include <glm/vec2.hpp>
#include <vector>

typedef std::vector<glm::vec2> vCoords;

struct GridGraph
{
	static bool isValid(glm::vec2 coord);
	static vCoords getNeighbors(glm::vec2 coord);
	static int manhattanDistance(glm::vec2 a, glm::vec2 b);
	/*
	 * Assumes origin and destination are two valid coordinates that
	 * are "adjacent" (generated by `getNeighbors`)
	 */
	static int getEdgeWeight(glm::vec2 origin, glm::vec2 destination);
	static void printCoord(glm::vec2 coord);
	static void printCoords(vCoords coords);
};

