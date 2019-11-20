#pragma once

#include <glm/vec3.hpp>
#include <vector>

double randInRange(double min, double max);

unsigned long long AABBHash(glm::vec3 testLocation);

template <typename T>
void stdVecConc(std::vector<T>* v1, std::vector<T>* v2)
{
	v1->insert(v1->end(), v2->begin(), v2->end());
}