#pragma once

#include "cShaderManager.h"
#include "DebugRenderer/cDebugRenderer.h"

extern cShaderManager theShaderManager;
extern std::string shader_name;

extern std::string scene_filename;

extern bool isPlaying;

extern cDebugRenderer* theDebugRenderer;

// These are defined in theMain.cpp
//extern glm::vec3 cameraEye;
//extern glm::vec3 cameraTarget;
//extern glm::vec3 upVector;
