#pragma once

#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include "iGameItem.h"

iGameItem* createGameItem(std::string type, nlohmann::json info);

void resetFactory(std::string);