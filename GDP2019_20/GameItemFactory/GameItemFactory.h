#pragma once

#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include "iGameItem.h"

aGameItem* createGameItem(std::string type, nlohmann::json info);

void resetFactory(std::string);