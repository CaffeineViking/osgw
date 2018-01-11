#version 410

in vec2 texmaps;

uniform sampler2D sampler;

out vec4 color;

void main() {
    vec4 texel = texture(sampler, texmaps);
    color = vec4(texel);
}
