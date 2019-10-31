#include "util.h"
#include <stdlib.h>

double randInRange(double min, double max)
{
	return min + rand() / (RAND_MAX / (max - min));
};
