#pragma once
#include <game_math.h>   // single include header for all glm math stuffs

// nCollide
//
// A namespace to isolate functions taken directly from
// REAL-TIME COLLISION DETECTION, ERICSON
//
// The functions have been slightly altered to:
// a) Return additional information about shapes "already" colliding.
// b) Implement the glm math library
namespace nCollide
{
	// closest_point_on_plane
	// Given a plane and any point in space, determine the closest point.
	// 
	// point : any point in space
	// n : plane normal
	// d : plane dot product
	// returns the closest point on the plane of point projected onto the plane
	glm::vec3 closest_point_on_plane(
		const glm::vec3& point, const glm::vec3& n, float d);
	// c : sphere center
	// r : sphere radius
	// v : sphere movement
	// n : plane normal
	// d : plane dot product
	// t (output) : [0,1] A ratio of the timestep, indicating the time of collision.
	// q (output) : The collision point, where the sphere first contacts the plane.
	// Returns one of three values: [-1,0,1]
	// Returns -1 if a collision was already happening at the beginning of the timestep.
	// Returns  0 if no collision happened during the timestep.
	// Returns  1 if a collision happened during the timestep.
	int intersect_moving_sphere_plane(
		const glm::vec3& c, float r,
		const glm::vec3& v, const glm::vec3& n, float d,
		float& t, glm::vec3& q);
	// point : Ray origin. Point in ray.
	// dir : Normalized direction vector parallel to ray.
	// center : Center of the sphere in world space.
	// radius : Length of radius of the sphere.
	// t (output) : Parametrized point in ray where there's an intersection.
	//              Lowest value in case of double intersection.
	// q (output) : Point in world space where ray intersects sphere.
	// Returns one of two values: [0,1]
	// Returns 0 in case of no intersection.
	// Returns 1 in case of intersection.
	int intersect_ray_sphere(
		const glm::vec3& point, const glm::vec3& dir,
		const glm::vec3& center, float radius,
		float& t, glm::vec3& q);
	// c0 : Position in world space of sphere 0.
	// r0 : Radius length of sphere 0.
	// v0 : Instantaneous velocity of sphere 0.
	// c1 : Position in world space of sphere 1.
	// r1 : Radius length of sphere 1.
	// v1 : Instantaneous velocity of sphere 1.
	// t (output) : [0,1] A ratio of the timestep, indicating the time of collision.
	// Returns one of three values: [-1,0,1]
	// Returns -1 if a collision was already happening at the beginning of the timestep.
	// Returns  0 if no collision happened during the timestep.
	// Returns  1 if a collision happened during the timestep.
	int intersect_moving_sphere_sphere(
		const glm::vec3& c0, float r0, const glm::vec3& v0,
		const glm::vec3& c1, float r1, const glm::vec3& v1, float& t);
}