// include/particle.h
#ifndef PARTICLE_H
#define PARTICLE_H

#include "../external/glm/glm.hpp"

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 color;
    float life;
};

#endif