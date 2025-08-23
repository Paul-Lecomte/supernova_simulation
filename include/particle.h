#pragma once
#include <vector>
struct Particle {
    float x, y, vx, vy;
    float r, g, b, a;
};
class ParticleSystem {
public:
    ParticleSystem();
    void update();
    void render();
private:
    std::vector<Particle> particles;
};