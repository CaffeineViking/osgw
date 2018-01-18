#version 410

in vec3 position;
in vec2 mapping;

uniform float time;
uniform mat4 model;
uniform mat4 perspective_view;

out vec2 pass_mapping;
out vec3 pass_position;

void main() {
    pass_mapping = mapping;
    vec4 homogenous = vec4(position, 1.0);
    mat4 pvm = perspective_view*model;
    vec4 final_pos = pvm*homogenous;
    pass_position = final_pos.xyz;
    gl_Position = final_pos;
}
