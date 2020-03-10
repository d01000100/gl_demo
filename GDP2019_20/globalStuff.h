#pragma once

#include "cShaderManager.h"
#include "cVAOManager.h"
#include "cBasicTextureManager.h"
#include "DebugRenderer/cDebugRenderer.h"
#include "cMazeMaker.h"
#include "cLowPassFilter.h"

extern cShaderManager theShaderManager;
extern cVAOManager* theVAOManager;
extern std::string shader_name;
extern cMazeMaker mazeMaker;
extern cLowPassFilter avgDeltaTimeThingy;

extern std::string scene_filename;

extern GLFWwindow* window;

extern cBasicTextureManager* g_pTextureManager;

extern cDebugRenderer* g_pDebugRenderer;

extern bool isDebug, isRunning, withCollisions;
