#include "BMPMap.h"
#include "BMPImage.h"
#include <iostream>
#include "ResourceManager.h"
#include <glm/gtx/string_cast.hpp>

/*
 * An array of the "rows" of the image
 */
char** BMPMap::map;
glm::vec2 BMPMap::dimensions, BMPMap::origin, BMPMap::resource, BMPMap::goal;

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
	map = new char*[dimensions.y];
	size_t index = 0;
	for (size_t posY = 0; posY < dimensions.y; posY++)
	{
		map[posY] = new char[dimensions.x];
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
			map[posY][posX] = color;

			if (color == 'g')
			{
				origin.x = posX; origin.y = posY;
			}

			if (color == 'r')
			{
				resource.x = posX; resource.y = posY;
			}

			if (color == 'b')
			{
				goal.x = posX; goal.y = posY;
			}
		}
		//std::cout << std::endl;
	}
	//std::cout << std::endl;
	return true;
}

char BMPMap::getColor(glm::vec2 coord)
{
	return map[(int)coord.y][(int)coord.x];
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
			std::cout << map[j][i] << ",";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "goal: " << glm::to_string(goal) << std::endl <<
		"origin: " << glm::to_string(origin) << std::endl <<
		"resource: " << glm::to_string(resource) << std::endl;
}