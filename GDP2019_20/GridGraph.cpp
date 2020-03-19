#include "GridGraph.h"
#include "BMPMap.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "util.h"

bool GridGraph::isValid(glm::vec2 coord)
{
	return coord.x >= 0 && coord.y >= 0 &&
		coord.x < BMPMap::dimensions.x &&
		coord.y < BMPMap::dimensions.y &&
		BMPMap::getNode(coord)->color != '_';
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

int GridGraph::manhattanDistance(glm::vec2 a, glm::vec2 b)
{
	return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

int GridGraph::getEdgeWeight(glm::vec2 origin, glm::vec2 destination)
{
	// We assume both coordinates are adjacent
	int distance = 10;
	// Check if it's diagonal
	if (origin.x != destination.x && origin.y != destination.y)
	{
		distance = 14;
	}
	// Check if destination is "difficult terrain"
	if (BMPMap::getNode(destination)->color == 'y')
	{
		distance *= 2;
	}
	return distance;
}

void GridGraph::printCoord(glm::vec2 coord)
{
	std::cout << "(" << coord.x << "," << coord.y << ") ";
}

void GridGraph::printCoords(vCoords coords)
{
	std::cout << "[";
	for (auto coord : coords)
	{
		printCoord(coord);
	}
	std::cout << "]\n";
}

bool IsNodeInList(vNodes openList, sNodeInfo* child)
{
	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i] == child)
			return true;

	}
	return false;
}

void vectorRemove(vNodes &coll, sNodeInfo* value)
{
	for (auto itColl = coll.begin(); itColl != coll.end(); itColl++)
	{
		if (*itColl == value)
		{
			coll.erase(itColl);
			return;
		}
	}
}

sNodeInfo* getLowestNotVisited(const vNodes &nodes)
{
	int lowest = std::numeric_limits<int>::max();
	sNodeInfo *res = nullptr;
	for (auto node : nodes)
	{
		if (!node->visited && node->distanceSoFar < lowest)
		{
			lowest = node->distanceSoFar;
			res = node;
		}
	}
	return res;
}

using namespace std;

vCoords GridGraph::dijkstra()
{
	BMPMap::resetGraph();
	auto rootNode = BMPMap::origin;
	rootNode->visited = true;
	rootNode->distanceSoFar = 0;
	vNodes closedList;
	vNodes openList;
	openList.push_back(rootNode);

	while (!openList.empty())
	{
		float minCost = FLT_MAX;
		int minIndex = 0;
		//find node with the lowest cost from root node
		for (size_t i = 0; i < openList.size(); i++)
		{
			if (openList[i]->distanceSoFar < minCost)
			{
				minCost = openList[i]->distanceSoFar;
				minIndex = i;
			}
		}

		//remove current node from open list and add to closed list
		sNodeInfo* currNode = openList[minIndex];
		for (auto iter = openList.begin(); iter != openList.end(); ++iter)
		{
			if (*iter == currNode)
			{
				openList.erase(iter);
				break;
			}
		}
		closedList.push_back(currNode);

		currNode->visited = true;
		if (currNode == BMPMap::resource)
		{
			break;
		}

		//Go through every child node node 
		for (auto neighborCoord : getNeighbors(currNode->coord))
		{
			auto neighbor = BMPMap::getNode(neighborCoord);
			if (neighbor->visited == false)
			{
				int weightSoFar = currNode->distanceSoFar + 
					getEdgeWeight(currNode->coord, neighborCoord);
				if (weightSoFar < neighbor->distanceSoFar)
				{
					//update node when new better path is found
					neighbor->distanceSoFar = weightSoFar;
					neighbor->parent = currNode;
					if (!IsNodeInList(openList, neighbor))
					{
						openList.push_back(neighbor); //add newly discovered node to open list
					}
				}
			}
		}
	}

	// Retrieve the route from the end to the start
	vCoords route;
	auto endNode = BMPMap::resource;
	if (endNode->parent == nullptr)
	{
		// we couldn't find a way to the end
		return route;
	}
	auto parent = endNode->parent;
	route.push_back(endNode->coord);
	while (parent != nullptr)
	{
		route.push_back(parent->coord);
		parent = parent->parent;
	}
	std::reverse(route.begin(), route.end());
	return route;
}

vCoords GridGraph::aStar()
{
	BMPMap::resetGraph();
	auto rootNode = BMPMap::resource;
	rootNode->visited = true;
	rootNode->distanceSoFar = 0;
	vNodes closedList;
	vNodes openList;
	openList.push_back(rootNode);

	while (!openList.empty())
	{
		float minCost = FLT_MAX;
		int minIndex = 0;
		//find node with the lowest cost from root node
		// considering manhattan distance to goal
		for (size_t i = 0; i < openList.size(); i++)
		{
			if (openList[i]->distanceSoFar + manhattanDistance(openList[i]->coord, BMPMap::goal->coord) < minCost)
			{
				minCost = openList[i]->distanceSoFar;
				minIndex = i;
			}
		}

		//remove current node from open list and add to closed list
		sNodeInfo* currNode = openList[minIndex];
		for (auto iter = openList.begin(); iter != openList.end(); ++iter)
		{
			if (*iter == currNode)
			{
				openList.erase(iter);
				break;
			}
		}
		closedList.push_back(currNode);

		currNode->visited = true;
		if (currNode == BMPMap::goal)
		{
			break;
		}

		//Go through every child node node 
		for (auto neighborCoord : getNeighbors(currNode->coord))
		{
			auto neighbor = BMPMap::getNode(neighborCoord);
			if (neighbor->visited == false)
			{
				int weightSoFar = currNode->distanceSoFar +
					getEdgeWeight(currNode->coord, neighborCoord);
				if (weightSoFar < neighbor->distanceSoFar)
				{
					//update node when new better path is found
					neighbor->distanceSoFar = weightSoFar;
					neighbor->parent = currNode;
					if (!IsNodeInList(openList, neighbor))
					{
						openList.push_back(neighbor); //add newly discovered node to open list
					}
				}
			}
		}
	}

	// Retrieve the route from the end to the start
	vCoords route;
	auto endNode = BMPMap::goal;
	if (endNode->parent == nullptr)
	{
		// we couldn't find a way to the end
		return route;
	}
	auto parent = endNode->parent;
	route.push_back(endNode->coord);
	while (parent != nullptr)
	{
		route.push_back(parent->coord);
		parent = parent->parent;
	}
	std::reverse(route.begin(), route.end());
	return route;
}