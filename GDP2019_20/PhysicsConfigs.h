#pragma once
/* Macro to interchange the libraries */
//#define MY_PHYSICS 
#define BULLET_PHYSICS

#include "physics/interfaces/iPhysicsFactory.h"

#ifdef MY_PHYSICS
#include <wrapper/myPhysics/cPhysicsFactory.h>
#pragma comment (lib, "MyPhysicsWrapper.lib")
#pragma comment (lib, "MyPhysicsLibrary.lib")
#endif

#ifdef BULLET_PHYSICS
#include <wrapper/bullet/cPhysicsFactory.h>
#pragma comment (lib, "BulletPhysicsWrapper.lib")
#endif

using namespace nPhysics;

extern iPhysicsFactory* g_PhysicsFactory;
extern iPhysicsWorld* g_PhysicsWorld;

// Initialize the factory and the world to all of the engine to use
void g_initPhysics();
// Clean up both the physics and the world
void g_destroyPhysics();