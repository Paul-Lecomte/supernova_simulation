#include "particle_system.h"
#include "../external/glm/glm.hpp"
#include <cstdlib>      // for rand()
#include <cmath>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <algorithm>

ParticleSystem::ParticleSystem(unsigned int maxParticles)
    : maxParticles(maxParticles)
{
    particles.reserve(maxParticles);
}

void ParticleSystem::spawnParticle() {
    if (particles.size() >= maxParticles)
        return;

    float minSpeed = 0.2f;
    float maxSpeed = 1.0f;

    float baseAngle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
    float angleOffset = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.2f; // ±0.1 rad

    float angle = baseAngle + angleOffset;
    float speed = minSpeed + (static_cast<float>(rand()) / RAND_MAX) * (maxSpeed - minSpeed);

    Particle p;
    p.position = glm::vec2(centerX, centerY);
    p.velocity = glm::vec2(cos(angle), sin(angle)) * speed;
    p.color = glm::vec3(1.0f, 0.5f, 0.0f); // orange
    p.life = 1.0f; // 1 second life

    particles.push_back(p);
}

void ParticleSystem::update(float deltaTime) {
    for (auto &p : particles) {
        // Direction of current movement
        glm::vec2 dir = glm::normalize(p.velocity);

        // Outward acceleration (explosion effect)
        float acceleration = 0.5f;
        p.velocity += dir * acceleration * deltaTime;

        // Gravity towards or away from center
        glm::vec2 toCenter = glm::vec2(centerX, centerY) - p.position;
        float gravityStrength = -0.2f; // negative = push outward, positive = pull inward
        p.velocity += toCenter * gravityStrength * deltaTime;

        // Update position and life
        p.position += p.velocity * deltaTime;
        p.life -= deltaTime;
    }

    // Remove dead particles
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [](Particle &p){ return p.life <= 0.0f; }),
        particles.end()
    );

    // Spawn new particle
    spawnParticle();
}

void ParticleSystem::render() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_POINTS);
    for (auto &p : particles) {
        glColor4f(p.color.r, p.color.g, p.color.b, p.life);
        glVertex2f(p.position.x, p.position.y);
    }
    glEnd();
}