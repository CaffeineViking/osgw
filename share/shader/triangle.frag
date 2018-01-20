#version 410

in vec3 interpolated_position;
in vec2 interpolated_texture_coordinate;

uniform sampler2D texture_sampler;

out vec4 fragment_color;

void main() {
    vec4 texture_texel = texture(texture_sampler, interpolated_texture_coordinate);
    if (texture_texel.r < 0.1 && texture_texel.g < 0.1 && texture_texel.b < 0.1)
        discard;
    fragment_color = vec4(texture_texel);
}
