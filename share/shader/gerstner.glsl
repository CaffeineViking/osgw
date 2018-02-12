#version 410

// Gerstner wave: also called a trochoidal wave, is a solution to the Euler equations
// for periodic surface gravity waves. It describes how a incompressible fluid should
// evolve assuming the distance to the ocean bottom is infinite. The function uses an
// X-Z plane, and decides the Y-coordinate (height of wave) for any point on it. This
// function also varies over time. For our real-time purposes, a FFT-variant is used.

vec3 gerstner_wave(vec2 position, float time, inout vec3 normal) {
    return vec3(position.s, sin(position.x + time), position.t);
}

uniform struct GerstnerWave {
    vec2 direction;
    float amplitude;
    float frequency;
    float phase_shift;
} gerstner_wave_parameter[4];
