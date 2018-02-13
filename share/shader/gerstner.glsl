#version 410

// Gerstner wave: also called a trochoidal wave, describes how an incompressible fluid
// should evolve through time and space assuming the distance from the seafloor to the
// ocean surface is infinite. This wave-function works on a 2-D plane, and outputs the
// height of the wave at (x,z) at some time point t. An additional property of the Ge-
// restner wave is that it accumulates vertices closer to the waves' crests, depending
// on the steepness parameter that is used (0 is just a sine wave, and it peaks at 1).
//
// How to use it: you should only call the `gerstner_wave' function, the rest are help
// functions. The 'position' argument should tell us where we are in the (x, z)-plane,
// and the 'time' argument is the current elapsed time of the simulation. The 'normal'
// argument should be the normal of the surface we are going to displace, this will be
// overwritten with the new normal after the displacement using the wave has happened.
// Finally, the return value is the new position (x', y', z') after the Gerstner wave.
//
// Example:       position = gerstner_wave(position.xz, elapsed_time, normal);
//
// Changing wave: there are two ways to change the appearance of the wave-function, by
// manually changing/adding/removing parameters in the 'gerstner_wave_parameters' list
// below, or by dynamically uploading the data via uniforms. Or a combination of both.
// Note: this is nowhere even near optimal code, but structured for understandability.

uniform struct GerstnerWave {
    vec2 direction;
    float amplitude;
    float steepness;
    float frequency;
    float speed;
} gerstner_waves[] = GerstnerWave[](
    GerstnerWave(vec2(1.0, 0.0), 0.5, 0.5, 1.0, 2.0)
);

float gerstner_wave_height(vec2 position, float time) {
    float wave_height = 0.0;
    for (int i = 0; i < gerstner_waves.length(); ++i) {
        wave_height += gerstner_waves[i].amplitude * sin(
            dot(position, gerstner_waves[i].direction)
            * gerstner_waves[i].frequency
            + time
            * gerstner_waves[i].speed);
    } return wave_height;
}

float gerstner_wave_crestx(vec2 position, float time) {
    float wave_crestx = 0.0;
    for (int i = 0; i < gerstner_waves.length(); ++i) {
        wave_crestx += gerstner_waves[i].amplitude * cos(
            dot(position, gerstner_waves[i].direction)
            * gerstner_waves[i].frequency
            + time
            * gerstner_waves[i].speed)
            * gerstner_waves[i].steepness
            * gerstner_waves[i].direction.x;
    } return wave_crestx;
}

float gerstner_wave_cresty(vec2 position, float time) {
    float wave_cresty = 0.0;
    for (int i = 0; i < gerstner_waves.length(); ++i) {
        wave_cresty += gerstner_waves[i].amplitude * cos(
            dot(position, gerstner_waves[i].direction)
            * gerstner_waves[i].frequency
            + time
            * gerstner_waves[i].speed)
            * gerstner_waves[i].steepness
            * gerstner_waves[i].direction.y;
    } return wave_cresty;
}

vec3 gerstner_wave_normal(vec2 nposition, float time) {
    vec3 wave_normal = vec3(0.0, 1.0, 0.0);
    for (int i = 0; i < gerstner_waves.length(); ++i) {
        wave_normal.x -= gerstner_waves[i].amplitude * cos(
            dot(nposition, gerstner_waves[i].direction)
            * gerstner_waves[i].frequency
            + time
            * gerstner_waves[i].speed)
            * gerstner_waves[i].frequency
            * gerstner_waves[i].direction.x;
        wave_normal.y -= gerstner_waves[i].amplitude * sin(
            dot(nposition, gerstner_waves[i].direction)
            * gerstner_waves[i].frequency
            + time
            * gerstner_waves[i].speed)
            * gerstner_waves[i].frequency
            * gerstner_waves[i].steepness;
        wave_normal.z -= gerstner_waves[i].amplitude * cos(
            dot(nposition, gerstner_waves[i].direction)
            * gerstner_waves[i].frequency
            + time
            * gerstner_waves[i].speed)
            * gerstner_waves[i].frequency
            * gerstner_waves[i].direction.y;
    } return wave_normal;
}

vec3 gerstner_wave(vec2 position, float time, inout vec3 wave_normal) {
    vec3 wave_position = vec3(position.x + gerstner_wave_crestx(position, time),
                              gerstner_wave_height(position, time),
                              position.y + gerstner_wave_cresty(position, time));
    wave_normal = gerstner_wave_normal(wave_position.xz, time);
    return wave_position;
}
