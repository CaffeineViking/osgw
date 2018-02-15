#version 410

// Blinn-Phong lighting:

uniform uint directional_lights_size = 0;
uniform vec3 ambient_light_color;
uniform struct DirectionalLight {
    vec3 direction;
    vec3 color;
} directional_lights[8];

uniform uint point_lights_size = 0;
uniform struct PointLight {
    vec3 position;
    vec3 color;
    float falloff;
} point_lights[8];

float diffuse(vec3 surface_normal, vec3 light_direction) {
    return max(dot(surface_normal, light_direction), 0.0);
}

float specular(vec3 surface_normal, vec3 light_direction,
               vec3 eye_direction, float surface_gloss) {
    vec3 halfway = normalize(light_direction + eye_direction);
    return pow(max(dot(surface_normal, halfway), 0.0),
               surface_gloss);
}

vec3 blinn_phong(vec3 surface_position, vec3 surface_normal,
                 vec3 surface_diffuse, vec3 surface_specular,
                 float surface_shininess, vec3 eye) {
    vec3 ambient_light  = ambient_light_color,
         diffuse_light  = vec3(0.0, 0.0, 0.0),
         specular_light = vec3(0.0, 0.0, 0.0);

    vec3 eye_direction = normalize(eye - surface_position);

    for (uint i = 0; i < directional_lights_size; ++i) {
        vec3 light_vector = directional_lights[i].direction;
        vec3 light_color = directional_lights[i].color;

        diffuse_light += light_color * surface_diffuse
                         * diffuse(surface_normal,
                                   light_vector);
        specular_light += light_color * surface_specular
                          * specular(surface_normal, light_vector,
                                eye_direction, surface_shininess);
    }

    for (uint i = 0; i < point_lights_size; ++i) {
        vec3 light_color = point_lights[i].color;
        vec3 light_vector = point_lights[i].position - surface_position;
        vec3 light_direction = normalize(light_vector);
        float light_distance = length(light_vector);
        float light_falloff = light_distance
                              * point_lights[i].falloff;

        diffuse_light += light_color * surface_diffuse
                         * diffuse(surface_normal,
                                   light_direction)
                         / light_falloff;
        specular_light += light_color * surface_specular
                          * specular(surface_normal, light_direction,
                                eye_direction, surface_shininess)
                          / light_falloff;
    }

    // We might need to clamp each RGB channel when L > 1.
    return ambient_light + diffuse_light + specular_light;
}
