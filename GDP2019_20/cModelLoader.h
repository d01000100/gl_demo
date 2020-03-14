#ifndef _cModelLoader_HG_2019_09_09_
#define _cModelLoader_HG_2019_09_09_

#include <string>	

#include "cMesh.h"
#include "JSON_IO.h"

class cModelLoader
{
public:
	cModelLoader();			// constructor
	~cModelLoader();		// destructor

	// Takes the filename to load
	// Returns by ref the mesh
	static bool LoadPlyModel(
		std::string filename,
		std::string friendlyName
	);

	static void LoadAllModels(std::vector<meshSettings>* vMeshes);
};

#endif //_cModelLoader_HG_2019_09_09_

