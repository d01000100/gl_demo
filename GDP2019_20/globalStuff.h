#pragma once

#include "cGameObject.h"

// returns NULL (0) if we didn't find it.
cGameObject* pFindObjectByFriendlyName( std::string name );
cGameObject* pFindObjectByFriendlyNameMap( std::string name );


// These are defined in theMain.cpp
extern glm::vec3 cameraEye;
extern glm::vec3 cameraTarget;
extern glm::vec3 upVector;

extern glm::vec3 sexyLightPosition;
extern float sexyLightConstAtten;
extern float sexyLightLinearAtten;  
extern float sexyLightQuadraticAtten;

extern float sexyLightSpotInnerAngle;
extern float sexyLightSpotOuterAngle;

// This is a "normalized" direction
// (i.e. the length is 1.0f)
extern glm::vec3 sexyLightSpotDirection;

extern bool bLightDebugSheresOn;
