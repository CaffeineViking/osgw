#version 410

// ===================================================================================
//
//                 Copyright (c) 2018 Erik Sven Vasconcelos Jansson
//
//                                 The MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of the
// software and associated documentation files (a "Software"), to deal in the Software
// without restriction, including without limitations the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell copies of the Softwares, and to
// permit persons to whom the Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice must be included in copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO WARRANTY OF MERCHANTABILITY, FITNESS FOR AN PARTICULAR
// PURPOSES AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTIONS WITH THE SOFTWARES OR THE
// USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// -----------------------------------------------------------------------------------
//
// Introduction: the Gerstner/trochoidal wave describes how an incompresible fluid sh-
// ould evolve through time and space, under the assumption that the distance from the
// seafloor to the ocean surface is infinite. This wave-function is applied on to a 2D
// plane, and gives the height of the wave at (x, z) for a timepoint. A distinguishing
// feature of the Gerstner wave is that points passed to it tend to accumulate towards
// the waves's crest depending on the steepness (0 is a sine wave, but it peaks at 1).
//
// Shader usage: only the 'gerstner_wave' function should be called, the rest are just
// helper functions. The 'position' argument should tell us where we are in the (x, z)
// plane, and the 'time' argument is the current elapsed time of the simulation. Also,
// the 'normal' should be given, this is the normal of the geometry we are displacing,
// and will be overwritten after the call to 'gerstner_wave' with the normal after the
// Gerstner wave displacement has been applied to the surface. The return value is the
// position (x', y', z') of the point (x, z) after the Gerstner wave has been applied.
//
// Example:      position = gerstner_wave(position.xz, elapsed_time, normal);
//
// Custom waves: there are two ways to change the look-and-feel of the above function:
// by manually adding/removing/changing parameters in the 'gerstner_waves' array below
// or by dynamically uploading the data via uniform. In both cases you'll also need to
// give the number of waves that should be active, by setting 'gerstner_waves_length'.
//
// Example of wave customization by array:
//
// uniform uint gerstner_waves_length = 1;
// uniform struct GerstnerWave {
//     vec2 direction;
//     float amplitude;
//     float steepness;
//     float frequency;
//     float speed;
// } gerstner_waves[1] = GerstnerWave[1](
//     GerstnerWave(vec2(1.0, 0.0), 1.0, 0.5, 1.0, 1.0)
// );
//
// Example of wave customization by uploading uniforms for each of the parameters:
//
// glUniform2f(glUniformLocation(sp,  "gerstner_waves[0].direction"), 1.0f, 0.0f);
// glUniform1f(glUniformLocation(sp,  "gerstner_waves[0].amplitude"), 1.0);
// glUniform1f(glUniformLocation(sp,  "gerstner_waves[0].steepness"), 0.5);
// glUniform1f(glUniformLocation(sp,  "gerstner_waves[0].frequency"), 1.0);
// glUniform1f(glUniformLocation(sp,  "gerstner_waves[0].speed"),     1.0);
// glUniform1ui(glUniformLocation(sp, "gerstner_waves_length"),       1);
//
// Acknowledgments: the shader is based on Finch's article "Effective Water Simulation
// from Physical Models", that can be found in the first volume of GPU Gems by NVIDIA.
// It is somewhat better optimized than a straight translation of the equations in Fi-
// nch's paper, but there are definitely still parts that can be improved further too.
// I'd also like to thank Stefan Gustavson for the 'Procedural Methods for Images' co-
// urse at LiU, which allowed me to spend time to research and construct this project.
//
// ===================================================================================

uniform uint gerstner_waves_length = 0;
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
              width = maximum_width * cos(theta),
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
