#version 410

// Gerstner wave: also called a trochoidal wave, is a solution to the Euler equations
// for periodic surface gravity waves. It describes how a incompressible fluid should
// evolve assuming the distance to the ocean bottom is infinite. The function uses an
// X-Z plane, and decides the Y-coordinate (height of wave) for any point on it. This
// function also varies over time. For our real-time purposes, a FFT-variant is used.

uniform struct GerstnerWave {
    vec2 direction;
    float amplitude;
    float steepness;
    float frequency;
    float phase_shift;
} gerstner_wave_parameters[1] = GerstnerWave[1](
    GerstnerWave(vec2(1, 0), 1.0, 0.1, 1.0, 1.0)
);

float gerstner_wave_crest(vec2 position, float axis, float time) {
    float crests = 0.0;
    for (int i = 0; i < gerstner_wave_parameters.length(); i+=1) {
        crests += gerstner_wave_parameters[i].steepness * axis *
                  gerstner_wave_parameters[i].amplitude * cos(
                  dot(position, gerstner_wave_parameters[i].direction)
                  * gerstner_wave_parameters[i].frequency
                  + time
                  * gerstner_wave_parameters[i].phase_shift);
    } return crests;
}

float gerstner_wave_height(vec2 position, float time) {
    float height = 0.0;
    for (int i = 0; i < gerstner_wave_parameters.length(); i+=1) {
        height += gerstner_wave_parameters[i].amplitude * sin(
                  dot(position, gerstner_wave_parameters[i].direction)
                  * gerstner_wave_parameters[i].frequency
                  + time
                  * gerstner_wave_parameters[i].phase_shift);
    } return height;
}

vec3 gerstner_wave(vec2 position, float time, inout vec3 normal) {
    return vec3(position.s + gerstner_wave_crest(position, position.s, time),
                gerstner_wave_height(position, time),
                position.t + gerstner_wave_crest(position, position.t, time));
}
