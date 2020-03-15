#include "cModelLoader.h"
#include "cVAOManager.h"

#include <iostream>			// cin cout
#include <fstream>		    // ifstream ofstream

cModelLoader::cModelLoader()			// constructor
{
	std::cout << "A cModelLoader() is created!" << std::endl;
	return;
}

cModelLoader::~cModelLoader()			// destructor
{
	std::cout << "A cModelLoader() is destroyed. How sad." << std::endl;
	return;
}

// Takes the filename to load
// Returns by ref the mesh
bool cModelLoader::LoadPlyModel(
	std::string filename, std::string friendlyName)
{
	auto newMesh = new cMesh();
	cVAOManager::addLoadedMesh(friendlyName,newMesh);
	std::ifstream theFile( filename.c_str() );
	if ( ! theFile.is_open() )
	{	
		// On no! Where's the file???
		std::cout << "Could not find the model file: " << filename << std::endl;
		newMesh->setLoadState(cMesh::eLoadState::error);
		return false;
	}

	// Scan the file until I get to "vertex", and stop
	std::string temp;
	//theFile >> temp;
	while ( theFile >> temp ) 
	{
		// Have it hit the word "vertex"?
		if ( temp == "vertex" )
		{
			break;		// Exit the while loop
		}
	}// while ( theFile >> temp ) 
	
	unsigned int numberOfVertices;
	theFile >> numberOfVertices;



	while (theFile >> temp)
	{
		// Have it hit the word "face"?
		if (temp == "face")
		{
			break;		// Exit the while loop
		}
	}// while ( theFile >> temp ) 

	unsigned int numberOfTriangles;
	theFile >> numberOfTriangles;

	while (theFile >> temp)
	{
		// Have it hit the word "end_header"?
		if (temp == "end_header")
		{
			break;		// Exit the while loop
		}
	}// while ( theFile >> temp ) 



	// Read all the vertices
	for (unsigned int index = 0; index != numberOfVertices; index++)
	{
		// -0.0312216 0.126304 0.00514924
		sPlyVertex tempVertex;

		// -0.0312216 0.126304 0.00514924
		theFile >> tempVertex.x >> tempVertex.y >> tempVertex.z;

		// Also load the normals
		theFile >> tempVertex.nx >> tempVertex.ny >> tempVertex.nz;

		theFile >> tempVertex.u >> tempVertex.v;

		// Add this temp vertex to the vector of vertices
		// (cMesh &theMesh)
		newMesh->vecVertices.push_back( tempVertex );
	}

	for (unsigned int index = 0; index != numberOfTriangles; index++)
	{
		// 3 166 210 265 
		int discardThis;
		sPlyTriangle tempTriangle;

		theFile >> discardThis 
			>> tempTriangle.vert_index_1 
			>> tempTriangle.vert_index_2
			>> tempTriangle.vert_index_3;

		// Add this triangle
		newMesh->vecTriangles.push_back(tempTriangle);

		sNiceTriangle* niceTri = new sNiceTriangle();
		sPlyVertex plyA, plyB, plyC;
		plyA = newMesh->vecVertices[tempTriangle.vert_index_1];
		plyB = newMesh->vecVertices[tempTriangle.vert_index_2];
		plyC = newMesh->vecVertices[tempTriangle.vert_index_3];

		niceTri->a.x = plyA.x;
		niceTri->a.y = plyA.y;
		niceTri->a.z = plyA.z;
			   
		niceTri->b.x = plyB.x;
		niceTri->b.y = plyB.y;
		niceTri->b.z = plyB.z;
			   
		niceTri->c.x = plyC.x;
		niceTri->c.y = plyC.y;
		niceTri->c.z = plyC.z;

		// Normal are commented because I won't need them yet
		niceTri->normal = glm::vec3(plyA.nx, plyA.ny, plyA.nz) +
			glm::vec3(plyB.nx, plyB.ny, plyB.nz) +
			glm::vec3(plyC.nx, plyC.ny, plyC.nz);
		niceTri->normal /= 3.0f;
		niceTri->normal = glm::normalize(niceTri->normal);

		newMesh->niceTriangles.push_back(niceTri);
	}

	newMesh->setLoadState(cMesh::eLoadState::in_cpu);
	std::cout << "Loaded model " << friendlyName << " in cpu\n";
	return true;
}

void cModelLoader::LoadAllModels(std::vector<meshSettings>* vMeshes)
{
	for (const auto meshInfo : *vMeshes)
	{
		LoadPlyModel(meshInfo.filename, meshInfo.name);
	}
	delete vMeshes;
}
