#version 410

// Vertex shader: here we basically just apply the MVP-matrix and forward the coordinates
// to the next shader pipeline (either a geometry, tessellation or fragment shader). Here
// we also assume the geometry is simple, because we are going to be tessellating it too.

in vec3 position;
in vec3 normal;
in vec2 texture_coordinate;

#pragma include("matrices.glsl")

out PipelineData {
    vec3 position;
    vec3 normal;
    vec2 texture_coordinate;
} vs_out;

void main() {
    vec4 rotate_normal = model * vec4(normal, 0.0);
    vec4 homogenous_position = vec4(position, 1.0);
    vec4 world_pos = model * homogenous_position;

    vs_out.position = world_pos.xyz;
    vs_out.normal = rotate_normal.xyz;
    vs_out.texture_coordinate = texture_coordinate;
}
