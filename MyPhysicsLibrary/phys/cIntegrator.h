#pragma once
#include <game_math.h>   // single include header for all glm math stuffs

namespace phys
{
	// cIntegrator
	// 
	// A single place for numerical integration schemes.
	//
	// Contains single entry-points for each supported integrations scheme.
	class cIntegrator
	{
	private:
		struct State;
		struct Derivative;
		// Struct to calculate specific states in time during RK4 integration
		struct sRK4State
		{
			glm::vec3 position, velocity, acceleration;
			sRK4State (glm::vec3 p, glm::vec3 v, glm::vec3 a) :
				position(p), velocity(v), acceleration(a) {}
			sRK4State () :
				position(glm::vec3(0)),
				velocity(glm::vec3(0)),
				acceleration(glm::vec3(0)) {}
		};
		/*
		 * Calculate the position and velocity of the next "frame".
		 * Get the initial data and return the new data in `sRK4State`s
		 */
		sRK4State EvalRK4(const sRK4State &initial, float deltaTime, const sRK4State &derivatives);

	public:
		// Constructor (default)
		cIntegrator();

		/*
		 * FOURTH-ORDER RUNGE-KUTTA Integration with constant acceleration
		 * pos: initial position of the body that's moving.
		 *      It will be overwritten with the integrated position
		 * vel: initial velocity of the body that's moving.
		 *      It will be overwritten with the integrated velocity
		 * acc: initial acceleration of the body that's moving.
		 *      It will be overwritten with the integrated acceleration
		 * dt:  Small period of time to integrate on. 
		 */
		void RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt);
		/*
		 * FOURTH-ORDER RUNGE-KUTTA Integration with an additional acceleration
		 * pos: initial position of the body that's moving.
		 *      It will be overwritten with the integrated position
		 * vel: initial velocity of the body that's moving.
		 *      It will be overwritten with the integrated velocity
		 * acc: initial acceleration of the body that's moving.
		 *      It will be overwritten with the integrated acceleration
		 * appliedAcc: An external acceleration applied to the object in this "instant".
		 * dt:  Small period of time to integrate on. 
		 */
		void RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt);
	};
}