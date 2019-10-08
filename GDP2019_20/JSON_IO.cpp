#include "JSON_IO.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "cMesh.h"

using json = nlohmann::json;

std::vector<meshSettings>* readMeshes(std::string filename) {
	std::ifstream i;
	i.open(filename);

	if (!i.is_open()) {
		printf("Didn't found %s file \n", filename.c_str());
		return NULL;
	}

	json jFile;
	i >> jFile;

	json::iterator jMeshes = jFile.find("Meshes");
	if (jMeshes == jFile.end()) {
		printf("No Meshes found!!\n");
		return NULL;
	}

	std::vector <meshSettings> *vMeshes = new std::vector <meshSettings>();
	for (json::iterator mesh = jMeshes->begin();
		mesh != jMeshes->end(); mesh++) {

		meshSettings mSettings;
		if (mesh->find("name") == mesh->end()) {
			printf("Mesh without name!!\n");
			continue;
		}
		mSettings.name = (*mesh)["name"].get<std::string>();
		if (mesh->find("filename") == mesh->end()) {
			printf("Mesh without filename!!\n");
			continue;
		}
		mSettings.filename = (*mesh)["filename"].get<std::string>();
		vMeshes->push_back(mSettings);
	}

	return vMeshes;
}