#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "../external/glm/glm.hpp"
#include <vector>
#include "particle.h"

class ParticleSystem {
public:
    ParticleSystem(unsigned int maxParticles);

    void spawnParticle();                    // old single-particle spawn
    void spawnParticles(unsigned int count = 5); // multi-particle spawn
    void update(float deltaTime);            // update all particles
    void render();                           // render all particles (OpenGL)

private:
    std::vector<Particle> particles;
    unsigned int maxParticles;

    // 3D spawn position (center of explosion)
    float centerX = 0.0f;
    float centerY = 0.0f;
    float centerZ = 0.0f;  // NEW: z-coordinate for 3D
};

#endif