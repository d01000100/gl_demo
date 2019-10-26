#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

// no rotation, no scale
// So it;s NOT a GameItem
class Particle {
public:
	Particle();
	glm::vec3 position, velocity;
	float lifetime;
	glm::vec4 color;
};

class ParticleEmitter
{
public:
	glm::vec3 acceleration;
	glm::vec3 location;

	// This lets us to "reset" the emitter without allocating a new object
	void Init(glm::vec3 min_vel,
   			  glm::vec3 max_vel,
   			  glm::vec3 min_delta_pos,
   			  glm::vec3 max_delta_pos,
   			  float min_life,
   			  float max_life,
   			  int min_particles_per_frame,
   			  int max_particles_per_frame);
				 
	void Step(float deltaTime);
	// a "modify" function to avoid copying the particles
	// because they'll be a LOT. 
	// Also it's faster because we're not allocating new memory
	void getParticles(std::vector<Particle*> &particles);
private:
	std::vector<Particle*> particles;
	glm::vec3 min_vel;
	glm::vec3 max_vel;
	glm::vec3 min_delta_pos;
	glm::vec3 max_delta_pos;
	float min_life;
	float max_life;
	int min_particles_per_frame;
	int max_particles_per_frame;

	/* Add a particle to the scene */
	void createParticle();

	static const unsigned int DEFAULT_NUM_PARTICLES;

	template <class T>
	T randInRange( T min, T max )
	{
		double value = 
			min + static_cast <double> (rand()) 
			/ ( static_cast <double> (RAND_MAX/(static_cast<double>(max-min) )));
		return static_cast<T>(value);
	};
};

