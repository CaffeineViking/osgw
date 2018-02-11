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
    float base_contribution = 4.0;
    float distance_contribution = 20.0;

    tc_out[gl_InvocationID].normal = tc_in[gl_InvocationID].normal;
    tc_out[gl_InvocationID].position = tc_in[gl_InvocationID].position;
    tc_out[gl_InvocationID].texture_coordinate = tc_in[gl_InvocationID].texture_coordinate;

    float distance_to_eye = distance(tc_in[gl_InvocationID].position, eye_position);
    float tessel_lod = 1.0 - smoothstep(0.0, 24.0, distance_to_eye); // 1 --> close.
    float tessellation_level = distance_contribution*tessel_lod + base_contribution;

    gl_TessLevelInner[0] = tessellation_level;
    gl_TessLevelInner[1] = tessellation_level;
    gl_TessLevelOuter[0] = tessellation_level; gl_TessLevelOuter[1] = tessellation_level;
    gl_TessLevelOuter[2] = tessellation_level; gl_TessLevelOuter[3] = tessellation_level;
}
