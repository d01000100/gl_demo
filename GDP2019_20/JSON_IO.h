#pragma once
#include <string>
#include <vector>
#include <map>

struct meshSettings {
	std::string name;
	std::string filename;
};

std::vector<meshSettings>* readMeshes(std::string f);

