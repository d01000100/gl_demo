#include "PhysicsConfigs.h"

using namespace nPhysics;

iPhysicsFactory* g_PhysicsFactory = nullptr;
iPhysicsWorld* g_PhysicsWorld = nullptr;

void g_initPhysics() {
    g_PhysicsFactory = new nPhysics::cPhysicsFactory();
    g_PhysicsWorld = g_PhysicsFactory->CreateWorld();
}

void g_destroyPhysics()
{
    delete g_PhysicsFactory;
    delete g_PhysicsWorld;
}