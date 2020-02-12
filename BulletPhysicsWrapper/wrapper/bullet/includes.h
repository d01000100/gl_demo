#pragma once
// Wrapper around "Bullet Physics" that implements
// the interfaces defined in "PhysicsInterfaces" in order
// for clients of the interface to consume "Bullet Physics"
// in a decoupling way.

#include "cPhysicsWorld.h"
#include "cBallComponent.h"
#include "cPlaneComponent.h"
#include "cPhysicsFactory.h"