#pragma once

#include "cShaderManager.h"
#include "cVAOManager.h"
#include "cBasicTextureManager.h"
#include "DebugRenderer/cDebugRenderer.h"
#include <glm/mat4x4.hpp>

extern cShaderManager theShaderManager;
extern cVAOManager* theVAOManager;
extern std::string shader_name;
extern GLuint g_programID;

extern std::string config_filename;

extern GLFWwindow* window;

extern cBasicTextureManager* g_pTextureManager;

extern cDebugRenderer* g_pDebugRenderer;

extern bool isDebug, isRunning, isNightvision;

extern glm::mat4 viewTransform, projTransform;
