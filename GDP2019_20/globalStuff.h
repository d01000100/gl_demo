#pragma once

#include "cShaderManager.h"
#include "cVAOManager.h"
#include "cBasicTextureManager.h"
#include <fmod/fmod.hpp>

extern cShaderManager theShaderManager;
extern cVAOManager* theVAOManager;
extern std::string shader_name;

extern std::string scene_filename;

extern FMOD::System* fmod_system;
extern GLFWwindow* window;

extern cBasicTextureManager* g_pTextureManager;

FMOD_VECTOR glm_2_fmod_vec(glm::vec3 v);
glm::vec3 fmod_vec_2_glm(FMOD_VECTOR);
void error_check(FMOD_RESULT);
