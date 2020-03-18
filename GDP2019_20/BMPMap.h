#pragma once
#include <string>
#include <glm/vec2.hpp>

struct BMPMap
{
	static char** map;
	static glm::vec2 dimensions, origin, resource, goal;
	static bool readImage(std::string filename);
	static char getColor(glm::vec2 coord);
	static void printMap();
};
