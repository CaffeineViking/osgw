#version 410

// Gerstner wave: also called a trochoidal wave, describes how an incompressible fluid
// should evolve through time and space assuming the distance from the seafloor to the
// ocean surface is infinite. This wave-function works on a 2-D plane, and outputs the
// height of the wave at (x, z) for some time point. An additional property of the Ge-
// restner wave is that it accumulates vertices closer to the wave's crests, depending
// on the steepness parameter that is used (0 is just a sine wave, but it peaks at 1).
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
// manually changing/adding/removing parameter in the 'gerstner_waves' array below, or
// by dynamically uploading the data via uniforms with size. Or a combination of both.
// i.e. glUniform1f(glGetUniformLocation(handle, "gerstner_waves[0].steepness", 0.5));
// or gerstner_waves[] = GerstnerWave[1](GerstnerWave(vec2(1,0), 1.0, 0.5, 1.0, 1.0));
// You can probably figure out other ways to feed the wave function with data as well.
//
// Note: this is nowhere near optimal code, it is structured this way for readability,
// and if you think you've found an better solution, then you are most likely correct.

uniform uint gerstner_waves_length = 8;
uniform struct GerstnerWave {
    vec2 direction;
    float amplitude;
    float steepness;
    float frequency;
    float speed;
} gerstner_waves[8];

vec3 gerstner_wave_normal(vec3 position, float time) {
    vec3 wave_normal = vec3(0.0, 1.0, 0.0);
    for (uint i = 0; i < gerstner_waves_length; ++i) {
        float proj = dot(position.xz, gerstner_waves[i].direction),
              phase = time * gerstner_waves[i].speed,
              psi = proj * gerstner_waves[i].frequency + phase,
              Af = gerstner_waves[i].amplitude *
                   gerstner_waves[i].frequency,
              alpha = Af * sin(psi);

        wave_normal.y -= gerstner_waves[i].steepness * alpha;

        float x = gerstner_waves[i].direction.x,
              y = gerstner_waves[i].direction.y,
              omega = Af * cos(psi);

        wave_normal.x -= x * omega;
        wave_normal.z -= y * omega;
    } return wave_normal;
}

vec3 gerstner_wave_position(vec2 position, float time) {
    vec3 wave_position = vec3(position.x, 0, position.y);
    for (uint i = 0; i < gerstner_waves_length; ++i) {
        float proj = dot(position, gerstner_waves[i].direction),
              phase = time * gerstner_waves[i].speed,
              theta = proj * gerstner_waves[i].frequency + phase,
              height = gerstner_waves[i].amplitude * sin(theta);

        wave_position.y += height;

        float maximum_width = gerstner_waves[i].steepness *
                              gerstner_waves[i].amplitude,
              width = maximum_width * cos(theta);
              x = gerstner_waves[i].direction.x,
              y = gerstner_waves[i].direction.y;

        wave_position.x += x * width;
        wave_position.z += y * width;
    } return wave_position;
}

vec3 gerstner_wave(vec2 position, float time, inout vec3 normal) {
    vec3 wave_position = gerstner_wave_position(position, time);
    normal = gerstner_wave_normal(wave_position, time);
    return wave_position; // Accumulated Gerstner Wave.
}
