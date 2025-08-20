<!-- PROJECT TITLE & BADGES -->
<p align="center">
  <img src="public/supernova.svg" alt="Logo" width="120" />
</p>
<h1 align="center">supernova_simulation</h1>
<p align="center">
  <strong>Visual Simulation of a Supernova in C++ & OpenGL</strong><br>
  <a href="https://github.com/Paul-Lecomte/supernova_simulation/stargazers">
    <img alt="GitHub stars" src="https://img.shields.io/github/stars/Paul-Lecomte/supernova_simulation?style=social">
  </a>
  <img alt="Tech Stack" src="https://img.shields.io/badge/C++-00599C?logo=c%2B%2B&logoColor=white">
  <img alt="Tech Stack" src="https://img.shields.io/badge/OpenGL-5586A4?logo=opengl&logoColor=white">
  <img alt="Tech Stack" src="https://img.shields.io/badge/GLFW-000?logo=glfw&logoColor=white">
  <img alt="License: GPLv3" src="https://img.shields.io/badge/License-GPLv3-blue.svg">
</p>

---

## What is supernova_simulation?

**supernova_simulation** is an educational project that allows you to visualize the dynamics of a supernova explosion in real-time using OpenGL.  
The goal is to provide a simple base to explore physics and real-time graphics rendering in C++.

<p align="center">
  <img src="public/supernova_demo.png" alt="Supernova Demo" width="400" />
</p>

---

## Features

- 2D/3D visualization of a supernova explosion
- Particle customization (count, speed, color…)
- Keyboard controls to interact with the simulation
- Clean and modular C++ code

---

## Tech Stack

- **Language**: C++23
- **Rendering**: OpenGL
- **Windowing**: GLFW
- **OpenGL Loader**: GLAD or GLEW (choose one)
- **Build System**: CMake

---

## Project Structure

```bash
supernova_simulation/
├── main.cpp           # Entry point of the simulation
├── CMakeLists.txt     # CMake configuration
├── README.md          # Documentation
└── public/            # Images, logos, static assets (optional)
```

---

## Getting Started

### Clone the repository

```bash
git clone https://github.com/Paul-Lecomte/supernova_simulation.git
cd supernova_simulation
```

### Install dependencies

Make sure you have installed:
- CMake
- A C++17 compiler
- OpenGL, GLFW, GLAD/GLEW (via vcpkg, conan, or your package manager)

### Build

```bash
cmake -B build
cmake --build build
```

### Run the simulation

```bash
./build/supernova_simulation
```

---

## Usage

- Run the executable to see the supernova.
- Use arrow keys or the mouse to interact (see on-screen instructions).

---

## Customization

- Modify parameters in `main.cpp` to adjust physics or rendering.
- Add your own shaders or visual effects.

---

## Roadmap

- [ ] Basic particle explosion rendering
- [ ] Interactive parameter menu
- [ ] Full 3D support
- [ ] Export images or video of the simulation

---

## Acknowledgments

This project is inspired by the beauty of astrophysical phenomena and a passion for real-time rendering.

Special thanks to:
- The OpenGL community
- GLFW and GLAD authors

---

## License

This project is licensed under the GNU GPL v3.  
See the [LICENSE](./LICENSE) file for details.
