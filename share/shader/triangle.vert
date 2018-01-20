#version 410

in vec3 position;
in vec2 texture_coordinate;

uniform float time;
uniform mat4 model;
uniform mat4 projection_view;

out PipelineData {
    vec4 position;
    vec2 texture_coordinate;
} vs_out;

void main() {
    mat4 pvm = projection_view * model;
    vs_out.texture_coordinate = texture_coordinate;
    vec4 homogenous_position = vec4(position, 1.0);
    vs_out.position = pvm * homogenous_position;

    gl_Position = vs_out.position;
}
