#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <vector>
#include "particle.h"

class ParticleSystem {
public:
    ParticleSystem(unsigned int maxParticles);

    void spawnParticle();                  // spawn one particle
    void update(float deltaTime);          // update all particles
    void render();                          // render all particles (OpenGL)

private:
    std::vector<Particle> particles;
    unsigned int maxParticles;

    float centerX = 0.0f;  // spawn position (center)
    float centerY = 0.0f;
};

#endif