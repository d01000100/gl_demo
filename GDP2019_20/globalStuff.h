#pragma once

#include "cShaderManager.h"
#include "cVAOManager.h"
#include "cBasicTextureManager.h"
#include "DebugRenderer/cDebugRenderer.h"

extern cShaderManager theShaderManager;
extern GLuint shaderProgID;
extern cVAOManager* theVAOManager;
extern std::string shader_name;

extern std::string scene_filename;

extern GLFWwindow* window;

extern cBasicTextureManager* g_pTextureManager;

extern cDebugRenderer* g_pDebugRenderer;

extern bool isDebug, isRunning;

extern float deltaTime;
