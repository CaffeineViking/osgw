#version 410

in vec3 position;
in vec3 normal;
in vec2 texture_coordinate;

uniform mat4 model;
uniform mat4 projection_view;

out PipelineData {
    vec3 position;
    vec3 normal;
    vec2 texture_coordinate;
} vs_out;

void main() {
    mat4 pvm = projection_view * model;
    vec4 rotate_normal = model * vec4(normal, 0.0);
    vec4 homogenous_position = vec4(position, 1.0);
    vec4 world_pos = model * homogenous_position;

    vs_out.position = world_pos.xyz;
    vs_out.normal = rotate_normal.xyz;
    vs_out.texture_coordinate = texture_coordinate;

    gl_Position = pvm * homogenous_position;
}
