#version 410

uniform vec3 ambient_light_color;
uniform struct DirectionalLight {
    vec3 direction;
    vec3 position;
} directional_lights[8];
uniform uint directional_lights_size;

uniform struct PointLight {
    vec3 position;
    vec3 color;
    float falloff;
} point_lights[8];
uniform uint point_lights_size;
