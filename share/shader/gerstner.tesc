#version 410

layout(vertices = 4) out;

in PipelineData {
    vec3 position;
    vec3 normal;
    vec2 texture_coordinate;
} tc_in[];

#pragma include("matrices.glsl")

out PipelineData {
    vec3 position;
    vec3 normal;
    vec2 texture_coordinate;
} tc_out[];

void main() {
    float edge_tessellation = 24.0;
    float max_tessel = 12.0; float min_tessel = 12.0;

    tc_out[gl_InvocationID].normal = tc_in[gl_InvocationID].normal;
    tc_out[gl_InvocationID].position = tc_in[gl_InvocationID].position;
    tc_out[gl_InvocationID].texture_coordinate = tc_in[gl_InvocationID].texture_coordinate;

    float distance_to_eye = distance(tc_in[gl_InvocationID].position, eye_position);
    float tessellation_level = max_tessel * (1.0 / clamp(distance_to_eye, 1, 15)) + min_tessel;

    gl_TessLevelInner[0] = tessellation_level;
    gl_TessLevelInner[1] = tessellation_level;
    gl_TessLevelOuter[0] = edge_tessellation;  gl_TessLevelOuter[1] = edge_tessellation;
    gl_TessLevelOuter[2] = edge_tessellation;  gl_TessLevelOuter[3] = edge_tessellation;
}
