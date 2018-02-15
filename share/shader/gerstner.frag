#version 410

// Fragment shader: now that the ocean is being animated with Gerstner waves, the only
// part that is left to do is shade it. Here we use a simple Blinn-Phong shading model
// that takes two base colors (ones for shallow waves and another for deep waves), and
// combines them relative to the height of the wave. After that, we apply some surface
// ripples, to increase the details of the waves without having to add extra geometry.
// This will modify the normal of the wave, so we need to re-calculate it (again yes).
// Finally, we also apply the skybox reflection on the water. We mix these components.

in PipelineData {
    vec3 position;
    vec3 normal;
    vec2 texture_coordinate;
} fs_in;

#pragma include("lighting.glsl")
#pragma include("textures.glsl")
#pragma include("matrices.glsl")
#pragma include("snoise3d.glsl")

out vec4 color;

void main() {
    vec3 deep_wave_color    = vec3(0, 0.188, 0.203),
         shallow_wave_color = vec3(0.152, 1, 0.737);

    float limit_wave_height = 1.28;
    float wave_depth = smoothstep(-limit_wave_height,
                                  +limit_wave_height,
                                  fs_in.position.y);
    vec3 diffuse_albedo = mix(deep_wave_color,
                              shallow_wave_color,
                              wave_depth);

    float fog = clamp(distance(look_at_point, fs_in.position) / 24, 0, 1);
    vec3 shading_color = blinn_phong(fs_in.position, fs_in.normal,
                                     diffuse_albedo, vec3(0.0f),
                                     0.0, eye_position);
    color = vec4(mix(shading_color, fog_color, fog), 1);
}
