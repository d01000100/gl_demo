#include "AABBGrid.h"
#include "util.h"

#include <glm/gtx/string_cast.hpp>

AABBGrid::AABBGrid()
{
	// things that work for the battlestar galactica model
	float AABBsize = 100.0f;
	int gridSize = 13; // we'll have 13 AABBs on each side of the Grid
	glm::vec3 mins = glm::vec3(-200, -200, -600);
	// min coordinates of the battlestar galactica model: (-195,-107,-516)
	// the maximun AABB will end at: 13*100 + mins
	// = 1100, 1100, 700, which surronds the galactica model that ends at (232, 53, 531) by a lot
	// (maybe that's a problem)

	for (int x = 0; x < gridSize; x++) {
		for (int y = 0; y < gridSize; y++) {
			for (int z = 0; z < gridSize; z++) 
			{
				glm::vec3 AABBpos(
					mins.x + (x * AABBsize),
					mins.y + (y * AABBsize),
					mins.z + (z * AABBsize)
				);

				cAABB* pAABB = new cAABB(AABBpos);

				grid[AABBHash(AABBpos)] = pAABB;

				//printf("Creating an AABB with key: %llu\n",
				//	AABBHash(AABBpos));
			}
		}
	}
}

void AABBGrid::Draw()
{
	for (std::map<unsigned long long, cAABB*>::iterator itAABB = grid.begin();
		itAABB != grid.end(); itAABB++)
	{
		itAABB->second->Draw();
	}
}

/* Draws the box that contains `pos` */
void AABBGrid::Draw(glm::vec3 pos)
{
	unsigned long long poshHash = AABBHash(pos);
	if (grid.find(poshHash) != grid.end()) {
		grid[poshHash]->Draw();
	}
	else
	{
		//printf("No AABB with key %llu\nThat corresponds to %s\n", poshHash, glm::to_string(pos).c_str());
	}
}

std::vector<sNiceTriangle> sliceTriangle(sNiceTriangle triangle)
{
	glm::vec3 midAB, midAC, midBC;
	midAB = (triangle.a + triangle.b) / 2.0f;
	midAC = (triangle.a + triangle.c) / 2.0f;
	midBC = (triangle.c + triangle.b) / 2.0f;

	std::vector<sNiceTriangle> res;

	// A corner
	sNiceTriangle tempTriangle;
	tempTriangle.a = triangle.a;
	tempTriangle.b = midAB;
	tempTriangle.c = midAC;
	res.push_back(tempTriangle);

	// B corner
	tempTriangle.a = triangle.b;
	tempTriangle.b = midAB;
	tempTriangle.c = midBC;
	res.push_back(tempTriangle);

	// C corner
	tempTriangle.a = triangle.c;
	tempTriangle.b = midBC;
	tempTriangle.c = midAC;
	res.push_back(tempTriangle);

	// Middle corner
	tempTriangle.a = midAB;
	tempTriangle.b = midBC;
	tempTriangle.c = midAC;
	res.push_back(tempTriangle);

	return res;
}

void getTriangleHashes(std::set<unsigned long long>* res, sNiceTriangle triangle)
{
	float AABBlength = 110; // TODO: hardcoded
	if (glm::distance(triangle.a, triangle.b) > AABBlength ||
		glm::distance(triangle.a, triangle.c) > AABBlength || 
		glm::distance(triangle.c, triangle.b) > AABBlength)
	{
		std::vector<sNiceTriangle> subTriangles = sliceTriangle(triangle);
		for (int t = 0; t < subTriangles.size(); t++) {
			getTriangleHashes(res, subTriangles[t]);
		}
		return;
	}
	else
	{
		res->insert(AABBHash(triangle.a));
		res->insert(AABBHash(triangle.b));
		res->insert(AABBHash(triangle.c));
		return;
	}
}

std::set<unsigned long long> getTriangleHashes(sNiceTriangle* triangle) {
	std::set<unsigned long long> res;
	getTriangleHashes(&res, *triangle);
	return res;
}


void AABBGrid::filterTriangles(cMesh* mesh)
{
	for (int t = 0; t < mesh->niceTriangles.size(); t++)
	{
		sNiceTriangle* triangle = mesh->niceTriangles[t];
		std::set<unsigned long long> tHashes = getTriangleHashes(triangle);
		for(std::set<unsigned long long>::iterator h = tHashes.begin();
			h != tHashes.end(); h++)
		{
			if (this->grid.find(*h) != this->grid.end())
			{
				grid[*h]->triangles.insert(triangle);
			}
		}
		
		// Simple vertex recognition
		/* 
		unsigned long long hashA, hashB, hashC;
		hashA = AABBHash(triangle->a);
		hashB = AABBHash(triangle->b);
		hashC = AABBHash(triangle->c);

		// TODO: Manage large triangles
		if (this->grid.find(hashA) != this->grid.end())
		{
			this->grid[hashA]->triangles.push_back(triangle);
		}

		if (this->grid.find(hashB) != this->grid.end())
		{
			this->grid[hashB]->triangles.push_back(triangle);
		}

		if (this->grid.find(hashC) != this->grid.end())
		{
			this->grid[hashC]->triangles.push_back(triangle);
		}*/
	}

	for (std::map<unsigned long long, cAABB*>::iterator itGrid = grid.begin();
		itGrid != grid.end();)
	{
		if (itGrid->second->triangles.empty())
		{
			std::map<unsigned long long, cAABB*>::iterator toDelete = itGrid;
			itGrid++;
			delete toDelete->second;
			grid.erase(toDelete->first);
		}
		else {
			itGrid++;
		}
	}
}

cAABB* AABBGrid::findAABB(glm::vec3 pos)
{
	unsigned long long hash = AABBHash(pos);
	if (grid.find(hash) != grid.end()) {
		return grid[hash];
	}
	else {
		return NULL;
	}
}