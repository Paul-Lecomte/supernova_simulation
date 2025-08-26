#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <vector>
#include "particle.h"

class ParticleSystem {
public:
    ParticleSystem(unsigned int maxParticles);

    void spawnParticle();                  // old single-particle spawn
    void spawnParticles(unsigned int count = 5); // new multi-particle spawn
    void update(float deltaTime);          // update all particles
    void render();                         // render all particles (OpenGL)

private:
    std::vector<Particle> particles;
    unsigned int maxParticles;

    float centerX = 0.0f;  // spawn position (center)
    float centerY = 0.0f;
};

#endif