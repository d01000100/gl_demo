#include "BMPMap.h"
#include "BMPImage.h"
#include <iostream>
#include "ResourceManager.h"
#include <glm/gtx/string_cast.hpp>

/*
 * An array of the "rows" of the image
 */
sNodeInfo*** BMPMap::nodeMap;
glm::vec2 BMPMap::dimensions;
sNodeInfo *BMPMap::origin, *BMPMap::resource, *BMPMap::goal;

ResourceManager gResourceManager;

char GetColourCharacter(unsigned char b, unsigned char g, unsigned char r)
{
	if (r == 255 && g == 0 && b == 0) return 'r';
	if (r == 0 && g == 255 && b == 0) return 'g';
	if (r == 0 && g == 0 && b == 255) return 'b';
	if (r == 255 && g == 255 && b == 255) return 'w';
	if (r == 255 && g == 255 && b == 0) return 'y';
	if (r == 0 && g == 0 && b == 0) return '_';
	return 'x';
}

bool BMPMap::readImage(std::string filename)
{
	BMPImage bmp(filename);
	char* data = bmp.GetData();
	dimensions.x = bmp.GetImageWidth();
	dimensions.y = bmp.GetImageHeight();
	nodeMap = new sNodeInfo**[dimensions.y];
	size_t index = 0;
	for (size_t posY = 0; posY < dimensions.y; posY++)
	{
		nodeMap[posY] = new sNodeInfo*[dimensions.x];
		for (size_t posX = 0; posX < dimensions.x; posX++)
		{
			unsigned char r = data[index++];
			unsigned char g = data[index++];
			unsigned char b = data[index++];

			//std::cout << "[" << (int)r << "," << (int)g << "," << (int)b << "] ";
			
			char color = GetColourCharacter(
				r,
				g,
				b
			);
			auto node = new sNodeInfo();
			node->coord = glm::vec2(posX, posY);
			node->color = color;
			nodeMap[posY][posX] = node;

			if (color == 'g')
			{
				origin = node;
			}

			if (color == 'r')
			{
				resource = node;
			}

			if (color == 'b')
			{
				goal = node;
			}
		}
		//std::cout << std::endl;
	}
	//std::cout << std::endl;
	return true;
}

sNodeInfo* BMPMap::getNode(glm::vec2 coord)
{
	return nodeMap[(int)coord.y][(int)coord.x];
}

void BMPMap::resetGraph()
{
	for (int i = 0; i < dimensions.x; i++)
	{
		for (int j = 0; j < dimensions.y; j++)
		{
			auto node = nodeMap[i][j];
			node->parent = nullptr;
			node->distanceSoFar = std::numeric_limits<int>::max();
			node->visited = false;
		}
	}
}

void BMPMap::printMap()
{
	// To print, we want to print the top row first
	// and then go down the rows.
	// The rows are the first "coordinate" of the matrix
	// and we go through them in reverse
	for (int j = dimensions.y - 1; j >= 0; j--)
	{
		// The columns are the second coordinate and we
		// go through them from right to left
		for (int i = 0; i < dimensions.x; i++)
		{
			std::cout << nodeMap[j][i]->color << ",";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "goal: " << glm::to_string(goal->coord) << std::endl <<
		"origin: " << glm::to_string(origin->coord) << std::endl <<
		"resource: " << glm::to_string(resource->coord) << std::endl;
}
