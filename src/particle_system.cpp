#include "particle_system.h"
#include "../external/glm/glm.hpp"
#include <cstdlib>      // for rand()
#include <cmath>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <algorithm>
#include <random>

ParticleSystem::ParticleSystem(unsigned int maxParticles)
    : maxParticles(maxParticles)
{
    particles.reserve(maxParticles);
}

// Fonction de bruit simple (remplace par Perlin si tu veux)
float simpleNoise(float x, float y, float z) {
    return sin(x * 2.1f + y * 1.3f + z * 0.7f);
}

// Spawn multiple particles per frame
void ParticleSystem::spawnParticles(unsigned int count) {
    static std::vector<glm::vec3> palette = {
        {1.0f, 0.8f, 0.2f},   // jaune/orange (soufre)
        {1.0f, 0.2f, 0.2f},   // rouge (soufre)
        {0.4f, 0.7f, 1.0f},   // bleu (fer)
        {1.0f, 1.0f, 1.0f},   // blanc (synchrotron)
        {0.7f, 0.3f, 0.8f},   // violet
        {0.7f, 1.0f, 0.5f},   // vert (poussière/gaz)
        {0.2f, 0.2f, 0.2f}    // sombre (poussière)
    };

    const int filamentCount = 18;
    static std::vector<glm::vec3> filamentDirs;
    if (filamentDirs.empty()) {
        for (int i = 0; i < filamentCount; ++i) {
            float theta = (float)i / filamentCount * 2.0f * 3.14159f;
            float phi = 0.7f + static_cast<float>(rand()) / RAND_MAX * 0.7f;
            filamentDirs.push_back(glm::vec3(
                sin(phi) * cos(theta),
                sin(phi) * sin(theta),
                cos(phi)
            ));
        }
    }

    for (unsigned int i = 0; i < count; ++i) {
        if (particles.size() >= maxParticles) return;

        int filamentIdx = rand() % filamentCount;
        glm::vec3 baseDir = filamentDirs[filamentIdx];

        float t = static_cast<float>(rand()) / RAND_MAX;
        glm::vec3 curve = glm::cross(baseDir, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec3 dir = glm::normalize(baseDir + curve * (t - 0.5f) * 0.7f);

        // Forme ovoïde
        float radius = 0.1f + t * 3.0f + (static_cast<float>(rand()) / RAND_MAX) * 0.2f;
        glm::vec3 offset = dir * radius;
        offset.y *= 1.3f;

        // --- Ajout de vagues et d'arcs ---
        float wave = sin(radius * 3.0f + filamentIdx) * 0.2f;
        float noise = simpleNoise(offset.x, offset.y, offset.z) * 0.3f;
        offset += glm::vec3(
            wave * cos(filamentIdx + t * 6.28f),
            noise,
            wave * sin(filamentIdx + t * 6.28f)
        );

        // --- Ajout d'anneaux et de boucles ---
        if (rand() % 100 < 10) { // 10% de particules en anneaux
            float angle = t * 6.28f;
            float ringRadius = 0.8f + 0.5f * sin(filamentIdx + t * 3.0f);
            offset.x += cos(angle) * ringRadius;
            offset.z += sin(angle) * ringRadius;
        }

        float dist = glm::length(offset);

        // Couleur selon la zone
        glm::vec3 color;
        if (dist < 0.4f) color = palette[3];
        else if (dist < 1.0f) color = glm::mix(palette[0], palette[5], t);
        else if (dist < 2.0f) color = glm::mix(palette[1], palette[2], t);
        else color = palette[1];
        if (rand() % 100 < 5) color = palette[6];

        float speed = 0.08f + t * 1.3f + (static_cast<float>(rand()) / RAND_MAX) * 0.12f;
        float life = 1.2f + t * 2.5f + (static_cast<float>(rand()) / RAND_MAX) * 0.7f;

        Particle p;
        p.position = glm::vec3(centerX, centerY, centerZ) + offset;
        p.velocity = dir * speed;
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

// C++
void ParticleSystem::render() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_POINTS);
    for (auto &p : particles) {
        float dist = glm::length(p.position - glm::vec3(centerX, centerY, centerZ));
        float alpha = std::clamp(p.life, 0.2f, 1.0f);

        // Taille selon la zone
        float size = 2.0f;
        if (dist < 0.4f) size = 6.0f;           // centre laiteux
        else if (dist < 1.0f) size = 4.0f;      // intérieur jaune/vert/blanc
        else if (dist < 2.0f) size = 3.0f;      // filaments
        else size = 2.0f;                       // périphérie

        glPointSize(size);
        glColor4f(p.color.r, p.color.g, p.color.b, alpha);
        glVertex3f(p.position.x, p.position.y, p.position.z);
    }
    glEnd();
}