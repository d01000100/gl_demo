#include "ParticleEmitter.h"

const unsigned int ParticleEmitter::DEFAULT_NUM_PARTICLES = 1000;

Particle::Particle() {
	position = glm::vec3(0.0f);
	velocity = glm::vec3(0.0f);
	lifetime = 0.0f;
	color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void ParticleEmitter::Init(glm::vec3 min_vel,
	glm::vec3 max_vel,
	glm::vec3 min_delta_pos,
	glm::vec3 max_delta_pos,
	float min_life,
	float max_life,
	int min_particles_per_frame,
	int max_particles_per_frame) 
{
	this->min_vel = min_vel;
	this->max_vel = max_vel;
	this->min_delta_pos = min_delta_pos;
	this->max_delta_pos = max_delta_pos;
	this->min_life = min_life;
	this->max_life = max_life;
	this->min_particles_per_frame = min_particles_per_frame;
	this->max_particles_per_frame = max_particles_per_frame;

	// Load the vector with the maximum number of particles

	// makes the vector an initial size to avoid dynamic allocating
	particles.reserve(ParticleEmitter::DEFAULT_NUM_PARTICLES);

	for (int i = 0; i < ParticleEmitter::DEFAULT_NUM_PARTICLES; i++) {
		Particle* part = new Particle();
		part->lifetime = 0.0f;
		particles.push_back(part);
	}
	return;
}

void ParticleEmitter::Step(float deltaTime) {
	// Loop through the particles.
	for (std::vector<Particle*>::iterator i = particles.begin(); 
		i != particles.end(); i++) {
		// if alive
		if ((*i)->lifetime > 0) {
			// Decrease the lifeTime by deltaTime
			(*i)->lifetime -= deltaTime;
			// Move the particle. Explicit Euler Integration (flex)
			(*i)->velocity += acceleration * deltaTime;
			(*i)->position += (*i)->velocity * deltaTime;
		}
	}

	int new_particles_num = randInRange<int>(min_particles_per_frame, max_particles_per_frame);

	for (int i = 0; i < new_particles_num; i++) {
		createParticle();
	}
}

void ParticleEmitter::getParticles(std::vector<Particle*>& out_particles) {
	out_particles.clear();
	out_particles.reserve(particles.size());

	// Theres has to be enough space there
	for (std::vector<Particle*>::iterator i = particles.begin();
		i != particles.end(); i++) {

		if ((*i)->lifetime > 0) {
			out_particles.push_back(*i);
		}
	}
	std::copy(particles.begin(), particles.end(), out_particles.begin());
}

void ParticleEmitter::createParticle() {
	// find the first dead particle.
	Particle* iPart;
	for (std::vector<Particle*>::iterator i = particles.begin(); 
		i != particles.end(); i++) {
		if ((*i)->lifetime < 0.0f) {
			
			(*i)->lifetime = randInRange<int>(min_life, max_life);

			(*i)->velocity.x = randInRange<float>(min_vel.x, max_vel.x);
			(*i)->velocity.y = randInRange<float>(min_vel.y, max_vel.y);
			(*i)->velocity.z = randInRange<float>(min_vel.z, max_vel.z);

			(*i)->velocity.x = location.x + randInRange<float>(min_delta_pos.x, max_delta_pos.x);
			(*i)->velocity.y = location.y + randInRange<float>(min_delta_pos.y, max_delta_pos.y);
			(*i)->velocity.z = location.z + randInRange<float>(min_delta_pos.z, max_delta_pos.z);

			return;
		}
	}
	return;
}
