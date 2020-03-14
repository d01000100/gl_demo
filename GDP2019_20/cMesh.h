#ifndef _cMesh_HG_
#define _cMesh_HG_

#include <vector>		// "smart array"
#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

// This structure matches the file, so that our life is
//   simpler and happier, and we all get along.
struct sPlyVertex
{
	float x, y, z;
	float nx, ny, nz;		// NOW with NORMALS!!
	float u, v;
};

struct sPlyTriangle
{
	unsigned int vert_index_1;
	unsigned int vert_index_2;
	unsigned int vert_index_3;
};

struct sNiceTriangle
{
	glm::vec3 a, b, c, normal;
};

struct cMesh
{
	enum class eLoadState
	{
		not_loaded,
		in_cpu,
		in_gpu,
		error
	};
	
	std::string filename;
	// Store the vertices
	std::vector<sPlyVertex> vecVertices;

	// Store the triangles
	std::vector<sPlyTriangle> vecTriangles;

	std::vector<sNiceTriangle*> niceTriangles;
	void setLoadState(eLoadState state)
	{
		// TODO: Thread safe
		loadState = state;
	}
	eLoadState getLoadState()
	{
		// TODO: Thread safe
		eLoadState res;
		res = loadState;
		return res;
	}
private:
	eLoadState loadState = eLoadState::not_loaded;
};

#endif 
