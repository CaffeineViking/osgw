#version 410

in vec3 position;
in vec2 mapping;

uniform float time;

out vec2 texmaps;

void main() {
    texmaps = mapping;
    vec3 new_position = position;
    new_position.x *= cos(time);
    new_position.y *= abs(cos(time));
    gl_Position = vec4(new_position, 1.0);
}
