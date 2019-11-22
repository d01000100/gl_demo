#include "util.h"
#include <stdlib.h>
#include <glm/gtx/string_cast.hpp>

double randInRange(double min, double max)
{
	return min + rand() / (RAND_MAX / (max - min));
};

/*
	Create an unsinged long long number combining the three coordinates
	of a position. (max value: 18 446744 073709 551615)

	The coordinates will be truncated to the closest multiple of 100 first.

	Then, they will be merged to create a decimal number that fits in an ull,
	coding each number in six consecutive digits of the ull:
	five maximum digits for each number (a max of 99,999 in positions).
	The most significant digit will represent the original sign of the number:
	0 for positive, 1 for negative.

	Example: 4560, -572, 8 = 004500 100600 000000
*/
unsigned long long AABBHash(glm::vec3 testLocation)
{
	//printf("recieved: %s\n", glm::to_string(testLocation).c_str());
	unsigned long long ID;		// x, y, z

	// truncate to nearest lesser 100 multiple
	unsigned long long ulX = ((unsigned long long) fabs(floor(testLocation.x / 10.0f))) * 10; 
	unsigned long long ulY = ((unsigned long long) fabs(floor(testLocation.y / 10.0f))) * 10;
	unsigned long long ulZ = ((unsigned long long) fabs(floor(testLocation.z / 10.0f))) * 10;
	//printf("truncate to nearest 100: X: %llu; Y: %llu; Z: %llu\n", ulX, ulY, ulZ);

	// Are these negative
	if (testLocation.x < 0.0f)
	{
		ulX += 100000;		// 000,000   100,000   100300
	}
	if (testLocation.y < 0.0f)
	{
		ulY += 100000;		// 000,000   100,000
	}
	if (testLocation.z < 0.0f)
	{
		ulZ += 100000;		// 000,000   100,000
	}
	//printf("add negative flag: X: %llu; Y: %llu; Z: %llu\n", ulX, ulY, ulZ);

	unsigned long long oneMillion = 1000000;

	// Side length of my box is 100
	//	32 bit ---> 0 to 4,000,000,000  9
	//	64 bit ---> 0 to 16   ,000,000    ,000,000   ,000,000
	ulX *= oneMillion * oneMillion;			// 100 * 100
	ulY *= oneMillion;

	ID = ulX + ulY + ulZ;
	//printf("final key: %llu\n", ID);

	// 300, 700, 2400 ->      300,000700,002,400
	// -300, 700, -2400 -> 100300,000700,102,400
	return ID;
}
