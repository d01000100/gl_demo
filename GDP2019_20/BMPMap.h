#pragma once
#include <string>
#include <glm/vec2.hpp>
#include <vector>

struct sNodeInfo
{
	bool visited = false;
	int distanceSoFar = std::numeric_limits<int>::max();
	sNodeInfo* parent = nullptr;
	glm::vec2 coord = glm::vec2(-1);
	char color = '_';
};

typedef std::vector<sNodeInfo*> vNodes;

struct BMPMap
{
	static sNodeInfo *origin, *resource, *goal;
	static glm::vec2 dimensions;
	static bool readImage(std::string filename);
	static void printMap();
	static sNodeInfo* getNode(glm::vec2 coord);
	static void resetGraph();
private:
	static sNodeInfo*** nodeMap;
};
