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
		// TODO: Define any structs used internally for specific schemes.
		//       Such as,
		struct State;
		struct Derivative;
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
		// TODO: Define any methods used interally for specific schemes.
		/*
		 * Calculate the position and velocity of the next "frame".
		 * Get the initial data and return the new data in `sRK4State`s
		 */
		sRK4State EvalRK4(const sRK4State &initial, float deltaTime, const sRK4State &derivatives);

	public:
		// Constructor
		cIntegrator();

		// TODO: Do you really want all these options? Do you want others?
		//       You really only need RK4 (or Velocity Verlet) for Project 2.

		// TODO: Fully comment the inputs/outputs of any
		//       integration method you do end up including.

		// EULER ALGORITHM
		void Euler(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt);
		void Euler(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt);
		// EULER-CROMER ALGORITHM
		void EulerCromer(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt);
		void EulerCromer(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt);
		// MIDPOINT ALGORITHM
		void MidPoint(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt);
		void MidPoint(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt);
		// VELOCITY-VERLET ALGORITHM <- with half-step acceleration lag
		void VelocityVerlet(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt);
		void VelocityVerlet(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt);
		/*
		 * FOURTH-ORDER RUNGE-KUTTA Integration with constant acceleration
		 */
		void RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt);
		/*
		 * FOURTH-ORDER RUNGE-KUTTA Integration with an additional acceleration
		 */
		void RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt);
	};
}