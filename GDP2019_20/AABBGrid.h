#pragma once
#include <map>
#include <vector>

#include "cAABB.h"
#include "cMesh.h"

std::vector<unsigned long long> getTriangleHashes(sNiceTriangle* triangle);

class AABBGrid
{
private:
	std::map<unsigned long long, cAABB*> grid;
public:
	AABBGrid();

	void filterTriangles(cMesh* mesh);
	void Draw();
	void Draw(glm::vec3);
};

