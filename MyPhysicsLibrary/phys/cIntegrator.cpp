#include "cIntegrator.h"  // My header
#include <iostream>

namespace phys
{
	cIntegrator::sRK4State cIntegrator::EvalRK4(const sRK4State &initial, float deltaTime, const sRK4State &derivatives)
	{
		sRK4State res;
		res.position = initial.position + derivatives.velocity*deltaTime;
		res.velocity = initial.velocity + derivatives.acceleration*deltaTime;
		res.acceleration = initial.acceleration;
		//std::cout << "RKEval - Position: " << glm::to_string(res.position) <<
		//	" Velocity: " << glm::to_string(res.velocity) << std::endl;
		return res;
	}

	cIntegrator::cIntegrator() = default;

	// EULER ALGORITHM
	void cIntegrator::Euler(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt)
	{
		// TODO: math!
	}

	void cIntegrator::Euler(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt)
	{
		// TODO: math!
	}

	// EULER-CROMER ALGORITHM
	void cIntegrator::EulerCromer(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt)
	{
		// TODO: math!
	}

	void cIntegrator::EulerCromer(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt)
	{
		// TODO: math!
	}

	// MIDPOINT ALGORITHM
	void cIntegrator::MidPoint(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt)
	{
		// TODO: math!
	}

	void cIntegrator::MidPoint(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt)
	{
		// TODO: math!
	}

	// VELOCITY-VERLET ALGORITHM <- with half-step acceleration lag
	void cIntegrator::VelocityVerlet(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt)
	{
		// TODO: math!
	}

	void cIntegrator::VelocityVerlet(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt)
	{
		// TODO: math!
	}

	// FOURTH-ORDER RUNGE-KUTTA
	void cIntegrator::RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt)
	{
		float halfDeltaTime = dt*0.5f;
		sRK4State state(pos,vel,acc);
		sRK4State a = EvalRK4(state,0.f,sRK4State());
		sRK4State b = EvalRK4(state,halfDeltaTime,a);
		sRK4State c = EvalRK4(state,halfDeltaTime,b);
		sRK4State d = EvalRK4(state,dt,c);

		glm::vec3 rkVel = (1.f/6.f)*(a.velocity+((b.velocity+c.velocity)*2.f)+d.velocity);
		glm::vec3 rkAcc = (1.f/6.f)*(a.acceleration+((b.acceleration+c.acceleration)*2.f)+d.acceleration);

		pos += rkVel * dt;
		vel += rkAcc * dt;
	}

	void cIntegrator::RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt)
	{
		float halfDeltaTime = dt*0.5f;
		sRK4State state(pos,vel,acc + appliedAcc);
		sRK4State a = EvalRK4(state,0.f,sRK4State());
		sRK4State b = EvalRK4(state,halfDeltaTime,a);
		sRK4State c = EvalRK4(state,halfDeltaTime,b);
		sRK4State d = EvalRK4(state,dt,c);

		glm::vec3 rkVel = (1.f/6.f)*(a.velocity+((b.velocity+c.velocity)*2.f)+d.velocity);
		glm::vec3 rkAcc = (1.f/6.f)*(a.acceleration+((b.acceleration+c.acceleration)*2.f)+d.acceleration);

		pos += rkVel * dt;
		vel += rkAcc * dt;
	}
}