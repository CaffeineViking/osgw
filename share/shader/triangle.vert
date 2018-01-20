#version 410

in vec3 position;
in vec2 texture_coordinate;

uniform mat4 model;
uniform mat4 projection_view;

out vec3 interpolated_position;
out vec2 interpolated_texture_coordinate;

void main() {
    mat4 pvm = projection_view*model;
    interpolated_texture_coordinate = texture_coordinate;
    vec4 homogenous_position = vec4(position, 1.0);
    vec4 final_position = pvm*homogenous_position;
    interpolated_position = final_position.xyz;
    gl_Position = final_position;
}
