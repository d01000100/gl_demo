#pragma once
// A single include header for everything
// 
// This library encapsulates the logic to simulate a collection of
// rigid 3d bodies moving and colliding in 3d space.
//
// Created by José Daniel Casado Aguilar.
//
// It depends on the "MathIncludes" project. Which helps us to encapsulate
// includes from glm math library.
//
// The library makes easy to include rigid body simulation to any other project
// that requires it, like a 3d video game. It focuses on getting enough
// looking gooding simulations while optimizing calculations for each frame.
// It's also intended to create an easy-to-use interface for possible clients.
//
// The "cWorld" is a container for different cRigidBody's. It manages their movement
// and collision simulation.
//
// The "cWorld" does not "own" the cRigidBody's. The client must create and delete
// them on their own.
//
// Each "cRigidBody" contains a position in world space, a velocity, acceleration and mass
// in order to simulate a realistic result. They also have a "eShape" in order to test for
// collisions. The library currently supports spheres and planes.
//
// Version 0.2: Added soft bodies: networks of "rigid bodies" connected to each other
// by a spring
// 
// All the files required to make full use of this library:
#include "eShapeType.h"  // enum class for all supported shape types
#include "iShape.h"      // iShape is the base interface class for all shapes
#include "cRigidBody.h"  // cRigidBody is this library's rigid body representation
#include "shapes.h"      // Concrete classes for all supported shape types
#include "cWorld.h"      // cWorld is the primary container and operator of a simulation
#include "cSoftBody.h"