// src/particle_system.cpp
// Simulation d'une supernova en 3D avec OpenGL
#include <GL/gl.h>
#include <GL/glext.h>
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif
#include "particle_system.h"
#include "../external/glm/glm.hpp"
#include <cstdlib>
#include <cmath>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <random>
#include "../external/soil/include/SOIL/SOIL.h" // Pour le chargement de texture

// --- Bruit simple pour déformations ---
// Simplex ou Perlin seraient mieux, mais plus complexes à implémenter
float simpleNoise(float x, float y, float z) {
    return sin(x * 2.1f + y * 1.3f + z * 0.7f);
}

static GLuint particleTextureId = 0;

void loadParticleTexture() {
    if (particleTextureId != 0) return;
    particleTextureId = SOIL_load_OGL_texture(
        "assets/particle2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
    );
    glBindTexture(GL_TEXTURE_2D, particleTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

ParticleSystem::ParticleSystem(unsigned int maxParticles)
    : maxParticles(maxParticles)
{
    particles.reserve(maxParticles);
    loadParticleTexture();
}

static bool explosionDone = false;
static float explosionTime = 0.0f;

// --- Génération des particules ---
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

        // --- Forme ovoïde ---
        float radius = 0.1f + t * 3.0f + (static_cast<float>(rand()) / RAND_MAX) * 0.2f;
        glm::vec3 offset = dir * radius;
        offset.y *= 1.3f;

        // --- Vagues et arcs ---
        float wave = sin(radius * 3.0f + filamentIdx) * 0.2f;
        float noise = simpleNoise(offset.x, offset.y, offset.z) * 0.3f;
        offset += glm::vec3(
            wave * cos(filamentIdx + t * 6.28f),
            noise,
            wave * sin(filamentIdx + t * 6.28f)
        );

        // --- Anneaux et boucles ---
        if (rand() % 100 < 10) {
            float angle = t * 6.28f;
            float ringRadius = 0.8f + 0.5f * sin(filamentIdx + t * 3.0f);
            offset.x += cos(angle) * ringRadius;
            offset.z += sin(angle) * ringRadius;
        }

        float dist = glm::length(offset);

        // --- Couleur selon la zone ---
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

// --- Mise à jour des particules et explosion ---
void ParticleSystem::update(float deltaTime) {
    explosionTime += deltaTime;

    // --- Explosion initiale ---
    if (!explosionDone) {
        spawnParticles(800); // burst initial
        for (auto& p : particles) {
            p.velocity *= 3.0f; // vitesse x3
        }
        explosionDone = true;
    }

    // --- Onde de choc lumineuse ---
    if (explosionTime < 0.3f) {
        for (int i = 0; i < 100; ++i) {
            Particle shock;
            float theta = ((float)rand() / RAND_MAX) * 2.0f * 3.14159f;
            float phi = ((float)rand() / RAND_MAX) * 3.14159f;
            glm::vec3 dir(
                sin(phi) * cos(theta),
                sin(phi) * sin(theta),
                cos(phi)
            );
            shock.position = glm::vec3(centerX, centerY, centerZ);
            shock.velocity = dir * (6.0f + ((float)rand() / RAND_MAX) * 2.0f);
            shock.color = {1.0f, 1.0f, 1.0f};
            shock.life = 0.3f;
            particles.push_back(shock);
        }
    }

    // --- Update classique ---
    for (auto &p : particles) {
        glm::vec3 dir = glm::normalize(p.velocity);
        float acceleration = 0.4f + static_cast<float>(rand()) / RAND_MAX * 0.2f;
        p.velocity += dir * acceleration * deltaTime;

        glm::vec3 toCenter = glm::vec3(centerX, centerY, centerZ) - p.position;
        float gravityStrength = -0.2f + static_cast<float>(rand()) / RAND_MAX * 0.1f;
        p.velocity += toCenter * gravityStrength * deltaTime;

        p.position += p.velocity * deltaTime;
        p.life -= deltaTime;
    }

    // --- Suppression des particules mortes ---
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [](Particle &p){ return p.life <= 0.0f; }),
        particles.end()
    );

    // --- Génération continue ---
    spawnParticles(5);
}

// --- Rendu OpenGL avec billboards texturés ---
void ParticleSystem::render() {
    // Flash central (pic lumineux)
    if (explosionTime < 0.1f) {
        glDisable(GL_TEXTURE_2D);
        glPointSize(30.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f - explosionTime * 10.0f);
        glBegin(GL_POINTS);
        glVertex3f(centerX, centerY, centerZ);
        glEnd();
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Correction ici
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, particleTextureId);

    // Pour chaque particule, dessiner un quad centré sur la particule
    for (auto &p : particles) {
        float dist = glm::length(p.position - glm::vec3(centerX, centerY, centerZ));
        float alpha = std::clamp(p.life, 0.2f, 1.0f);

        float size = 2.0f;
        if (dist < 0.4f) size = 6.0f;
        else if (dist < 1.0f) size = 4.0f;
        else if (dist < 2.0f) size = 3.0f;
        else size = 2.0f;

        glColor4f(p.color.r, p.color.g, p.color.b, alpha);

        glPushMatrix();
        glTranslatef(p.position.x, p.position.y, p.position.z);

        // Billboard simple (pas de rotation, car la caméra ne tourne que sur Y)
        glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex3f(-size, -size, 0);
        glTexCoord2f(1,0); glVertex3f( size, -size, 0);
        glTexCoord2f(1,1); glVertex3f( size,  size, 0);
        glTexCoord2f(0,1); glVertex3f(-size,  size, 0);
        glEnd();

        glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // reset blending
}