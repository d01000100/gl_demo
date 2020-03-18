#include "GridGraph.h"
#include "BMPMap.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

bool GridGraph::isValid(glm::vec2 coord)
{
	return coord.x >= 0 && coord.y >= 0 &&
		coord.x < BMPMap::dimensions.x &&
		coord.y < BMPMap::dimensions.y &&
		BMPMap::getColor(coord) != '_';
}

void addValidCoord(glm::vec2 newCoord, vCoords& coords)
{
	if (GridGraph::isValid(newCoord))
	{
		coords.push_back(newCoord);
	}
}

vCoords GridGraph::getNeighbors(glm::vec2 coord)
{
	vCoords res;
	glm::vec2 upDir = glm::vec2(0, 1);
	glm::vec2 rightDir = glm::vec2(1, 0);
	glm::vec2 downDir = glm::vec2(0, -1);
	glm::vec2 leftDir = glm::vec2(-1,0);
	glm::vec2 up = coord + upDir;
	glm::vec2 down = coord + downDir;
	glm::vec2 right = coord + rightDir;
	glm::vec2 left = coord + leftDir;
	// "Straight" directions
	addValidCoord(up, res);
	addValidCoord(right, res);
	addValidCoord(down, res);
	addValidCoord(left, res);
	// Diagonals. Only neighbors if both "straight" directions that
	// make the diagonal are valid
	if (isValid(up) && isValid(right))
	{
		addValidCoord(coord + upDir + rightDir, res);
	}
	if (isValid(down) && isValid(right))
	{
		addValidCoord(coord + downDir + rightDir, res);
	}
	if (isValid(down) && isValid(left))
	{
		addValidCoord(coord + downDir + leftDir, res);
	}
	if (isValid(up) && isValid(left))
	{
		addValidCoord(coord + upDir + leftDir, res);
	}
	return res;
}

float GridGraph::manhattanDistance(glm::vec2 a, glm::vec2 b)
{
	return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

float GridGraph::getEdgeWeight(glm::vec2 origin, glm::vec2 destination)
{
	// We assume both coordinates are adjacent
	return 0;
}

void GridGraph::printCoords(vCoords coords)
{
	std::cout << "[";
	for (auto coord : coords)
	{
		std::cout << "(" << coord.x << "," << coord.y << ") ";
	}
	std::cout << "]\n";
}
