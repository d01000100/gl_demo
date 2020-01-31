#pragma once
// Wrapper around "MyPhysicsLibrary" that implements
// the interfaces defined in "PhysicsInterfaces" in order
// for clients of the interface to consume "MyPhysicsLibrary"
// in a decoupling way.

#include "cPhysicsWorld.h"
#include "cBallComponent.h"
#include "cPlaneComponent.h"
#include "cPhysicsFactory.h"