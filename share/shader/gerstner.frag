#version 410

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
    /* if (diffuse_texel.r < 0.3 && */
    /*     diffuse_texel.g < 0.3 && */
    /*     diffuse_texel.b < 0.3) discard; */

    color = diffuse_texel;
}
