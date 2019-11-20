#pragma once
#include <map>
#include <set>

#include "cAABB.h"
#include "cMesh.h"

std::set<unsigned long long> getTriangleHashes(sNiceTriangle* triangle);

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

