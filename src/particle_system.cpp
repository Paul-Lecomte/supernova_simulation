#include "particle_system.h"
#include "../external/glm/glm.hpp"
#include <cstdlib>
#include <cmath>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

// Constructor to initialize the particle system with a maximum number of particles
ParticleSystem::ParticleSystem(unsigned int maxParticles)
    : maxParticles(maxParticles) {
    particles.reserve(maxParticles);
}

void ParticleSystem::spawnParticle() {
    if (particles.size() >= maxParticles)
        return;

    float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
    float speed = static_cast<float>(rand()) / RAND_MAX * 0.5f;

    Particle p;
    p.position = glm::vec2(centerX, centerY);
    p.velocity = glm::vec2(cos(angle), sin(angle)) * speed;
    p.color = glm::vec3(1.0f, 0.5f, 0.0f); // Orange color
    p.life = 1.0f; // Particle life in seconds

    particles.push_back(p);
}