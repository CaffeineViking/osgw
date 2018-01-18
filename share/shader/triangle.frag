#version 410

in vec2 pass_mapping;
in vec3 pass_position;

uniform sampler2D sampler;

out vec4 color;

void main() {
    vec4 texel = texture(sampler, pass_mapping);
    color = vec4(texel);
}
