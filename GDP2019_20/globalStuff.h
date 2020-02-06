#pragma once

#include "cShaderManager.h"
#include "cVAOManager.h"
#include "cBasicTextureManager.h"
#include "DebugRenderer/cDebugRenderer.h"
#include <fmod/fmod.hpp>
#include <glm/mat4x4.hpp>

extern cShaderManager theShaderManager;
extern cVAOManager* theVAOManager;
extern std::string shader_name;
extern GLuint g_programID;

extern std::string scene_filename;

extern FMOD::System* fmod_system;
extern GLFWwindow* window;

extern cBasicTextureManager* g_pTextureManager;

extern cDebugRenderer* g_pDebugRenderer;

extern bool isDebug, isRunning;

extern glm::mat4 viewTransform, projTransform;

FMOD_VECTOR glm_2_fmod_vec(glm::vec3 v);
glm::vec3 fmod_vec_2_glm(FMOD_VECTOR);
void error_check(FMOD_RESULT);
