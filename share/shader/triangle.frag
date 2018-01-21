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
    if (diffuse_texel.r < 0.25 &&
        diffuse_texel.g < 0.25 &&
        diffuse_texel.b < 0.25) discard;

    vec4 opacity_texel = texture(opacity_map, 10.0*fs_in.texture_coordinate);
    if (opacity_texel.r < 0.25) discard;

    vec3 thresholds = vec3(sin(time), cos(time), sin(time));
    vec3 disco_texel = mix(diffuse_texel.rgb, fs_in.position, thresholds);
    color = vec4(disco_texel, 1.0); // Blend b.w texture and the position.
}
