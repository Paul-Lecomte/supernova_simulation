#include <GL/gl.h>
#include <GL/glu.h>    // <-- nécessaire pour gluPerspective et gluLookAt
#include <GLFW/glfw3.h>
#include <iostream>
#include "particle_system.h"

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Supernova Simulation 3D", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    ParticleSystem ps(2000);  // max 2000 particles for denser nebula

    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Clear color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set viewport and projection
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, width / (float)height, 0.1, 100.0); // FOV, aspect, near, far

        // Camera setup
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            0.0, 0.0, 15.0,   // camera position
            0.0, 0.0, 0.0,    // looking at explosion center
            0.0, 1.0, 0.0     // up vector
        );

        // Optionally rotate camera slowly around Y-axis
        static float angle = 0.0f;
        angle += deltaTime * 10.0f; // degrees per second
        glRotatef(angle, 0.0f, 1.0f, 0.0f);

        // Update and render particle system
        ps.update(deltaTime);
        ps.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}