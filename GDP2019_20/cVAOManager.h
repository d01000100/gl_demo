#ifndef _cVAOManager_HG_
#define _cVAOManager_HG_

#include "GLCommon.h"

// Will load the models and place them 
// into the vertex and index buffers to be drawn

#include <string>
#include <map>
#include "cMesh.h"				// File as loaded from the file
#include "sModelDrawInfo.h"

// The vertex structure 
//	that's ON THE GPU (eventually) 
// So dictated from THE SHADER
struct sVertex
{
	float x, y, z, w;			// w coordinate	
	float r, g, b, a;	// a = alpha (transparency)
	float nx, ny, nz, nw;
	float u0, v0, u1, v1;
};

struct cVAOManager
{

	// Takes a cMesh object and copies it into the GPU (as a VOA)
	bool LoadModelIntoVAO(std::string fileName, 
						  cMesh &theMesh,				// NEW
						  sModelDrawInfo &drawInfo, 
						  unsigned int shaderProgramID);

	// We don't want to return an int, likely
	bool FindDrawInfoByModelName(std::string filename,
								 sModelDrawInfo &drawInfo);

	std::string getLastError(bool bAndClear = true);

	std::map< std::string /*model name*/,
		      sModelDrawInfo /* info needed to draw*/ >
		m_map_ModelName_to_VAOID;

	bool LoadModelDrawInfoIntoVAO(
		sModelDrawInfo& drawInfo,
		unsigned int shaderProgramID);

};

#endif	// _cVAOManager_HG_
