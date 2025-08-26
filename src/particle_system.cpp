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
        float speed = minSpeed + static_cast<float>(rand()) / RAND_MAX * (maxSpeed - minSpeed);

        // Random 3D direction (spherical coordinates)
        float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;  // azimuth
        float phi   = static_cast<float>(rand()) / RAND_MAX * 3.14159f;         // elevation
        glm::vec3 dir(
            sin(phi) * cos(theta),
            sin(phi) * sin(theta),
            cos(phi)
        );

        // Slight random offset for filament-like appearance
        glm::vec3 offset = dir * (static_cast<float>(rand()) / RAND_MAX * 0.2f);

        // Random life
        float life = 1.0f + static_cast<float>(rand()) / RAND_MAX * 0.5f; // 1.0–1.5s

        // Randomized color (yellow → orange → red gradient)
        glm::vec3 color(
            1.0f,                              // red
            0.5f + 0.5f * (life / 1.5f),       // green fades over life
            0.2f * (1.0f - life / 1.5f)        // blue appears at end
        );

        Particle p;
        p.position = glm::vec3(centerX, centerY, centerZ) + offset;
        p.velocity = dir * speed;

        // Add small random turbulence
        p.velocity += glm::vec3(
            (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.1f,
            (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.1f,
            (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.1f
        );

        p.color = color;
        p.life = life;

        particles.push_back(p);
    }
}

void ParticleSystem::update(float deltaTime) {
    for (auto &p : particles) {
        // Direction of current movement
        glm::vec3 dir = glm::normalize(p.velocity);

        // Outward acceleration (explosion effect)
        float acceleration = 0.4f + static_cast<float>(rand()) / RAND_MAX * 0.2f; // 0.4–0.6
        p.velocity += dir * acceleration * deltaTime;

        // Gravity towards or away from center
        glm::vec3 toCenter = glm::vec3(centerX, centerY, centerZ) - p.position;
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
        glVertex3f(p.position.x, p.position.y, p.position.z);
    }
    glEnd();
}