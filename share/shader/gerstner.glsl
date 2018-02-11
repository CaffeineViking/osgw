#version 410

#pragma include("sineprng.glsl")

// Gerstner wave: also called a trochoidal wave, is a solution to the Euler equations
// for periodic surface gravity waves. It describes how a incompressible fluid should
// evolve assuming the distance to the ocean bottom is infinite. The function uses an
// X-Z plane, and decides the Y-coordinate (height of wave) for any point on it. This
// function also varies over time. For our real-time purposes, a FFT-variant is used.

float gerstner_wave(vec2 position, float time) {
    return sin(position.x + time);
}
