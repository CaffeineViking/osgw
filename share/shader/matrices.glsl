#version 410

// Global uniforms: things that are usually required by all shader programs to
// work at all. The time is counted in seconds, and everything is in world co-
// ordinates. The projection-view matrix should be provided between S-P calls.

uniform float time;
uniform mat4 model;
uniform vec4 fog_diffuse;
uniform vec3 eye_position;
uniform mat4 projection_view;
