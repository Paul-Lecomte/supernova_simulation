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

// Spawn multiple particles per frame
void ParticleSystem::spawnParticles(unsigned int count) {
    for (unsigned int i = 0; i < count; ++i) {
        if (particles.size() >= maxParticles) return;

        // Random speed
        float minSpeed = 0.2f;
        float maxSpeed = 1.0f;
        float speed = minSpeed + (static_cast<float>(rand()) / RAND_MAX) * (maxSpeed - minSpeed);

        // Random angle with slight offset
        float baseAngle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
        float angleOffset = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.4f; // ±0.2 rad
        float angle = baseAngle + angleOffset;

        // Random life
        float life = 1.0f + static_cast<float>(rand()) / RAND_MAX * 0.5f; // 1.0–1.5s

        // Randomized color (orange-yellow gradient)
        glm::vec3 color(
            0.8f + static_cast<float>(rand()) / RAND_MAX * 0.2f, // red
            0.3f + static_cast<float>(rand()) / RAND_MAX * 0.2f, // green
            0.0f                                               // blue
        );

        Particle p;
        p.position = glm::vec2(centerX, centerY);
        p.velocity = glm::vec2(cos(angle), sin(angle)) * speed;
        p.color = color;
        p.life = life;

        particles.push_back(p);
    }
}

void ParticleSystem::update(float deltaTime) {
    for (auto &p : particles) {
        // Direction of current movement
        glm::vec2 dir = glm::normalize(p.velocity);

        // Outward acceleration (explosion effect)
        float acceleration = 0.4f + static_cast<float>(rand()) / RAND_MAX * 0.2f; // 0.4–0.6
        p.velocity += dir * acceleration * deltaTime;

        // Gravity towards or away from center
        glm::vec2 toCenter = glm::vec2(centerX, centerY) - p.position;
        float gravityStrength = -0.2f + static_cast<float>(rand()) / RAND_MAX * 0.1f; // -0.2 to -0.1
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

    // Spawn new particles automatically (burst)
    spawnParticles(5); // 5 per frame, tweak as needed
}

void ParticleSystem::render() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_POINTS);
    for (auto &p : particles) {
        // Fade particles as life decreases
        float alpha = p.life;
        glColor4f(p.color.r * alpha, p.color.g * alpha, p.color.b * alpha, alpha);
        glVertex2f(p.position.x, p.position.y);
    }
    glEnd();
}