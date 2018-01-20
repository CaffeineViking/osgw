#version 410

layout(vertices = 3) out;

in PipelineData {
    vec4 position;
    vec2 texture_coordinate;
} tc_in[];

out PipelineData {
    vec4 position;
    vec2 texture_coordinate;
} tc_out[];

void main() {
    tc_out[gl_InvocationID].texture_coordinate = tc_in[gl_InvocationID].texture_coordinate;
    tc_out[gl_InvocationID].position = tc_in[gl_InvocationID].position;

    gl_TessLevelInner[0] = 1;
    gl_TessLevelOuter[0] = 1;
    gl_TessLevelOuter[1] = 1;
    gl_TessLevelOuter[2] = 1;
}
