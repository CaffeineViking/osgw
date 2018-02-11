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
    vec4 diffuse_texel = texture(diffuse_map, fs_in.texture_coordinate);
    float distance_to_eye = distance(fs_in.position, eye_position);
    float fog_ratio = smoothstep(12.0, 24.0, distance_to_eye);
    color = mix(diffuse_texel, fog_diffuse, fog_ratio);
}
