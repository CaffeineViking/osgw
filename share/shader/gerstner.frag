#version 410

// Fragment shader:

in PipelineData {
    vec3 position;
    vec3 normal;
    vec2 texture_coordinate;
} fs_in;

#pragma include("lighting.glsl")
#pragma include("textures.glsl")
#pragma include("matrices.glsl")

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
