#version 410

layout(triangles, fractional_odd_spacing) in;

in PipelineData {
    vec3 position;
    vec3 normal;
    vec2 texture_coordinate;
} te_in[];

#pragma include("matrices.glsl")

out PipelineData {
    vec3 position;
    vec3 normal;
    vec2 texture_coordinate;
} te_out;

void main() {
    te_out.normal  = gl_TessCoord.x * te_in[0].normal;
    te_out.normal += gl_TessCoord.y * te_in[1].normal;
    te_out.normal += gl_TessCoord.z * te_in[2].normal;

    te_out.texture_coordinate  = gl_TessCoord.x * te_in[0].texture_coordinate;
    te_out.texture_coordinate += gl_TessCoord.y * te_in[1].texture_coordinate;
    te_out.texture_coordinate += gl_TessCoord.z * te_in[2].texture_coordinate;

    te_out.position  = gl_TessCoord.x * te_in[0].position;
    te_out.position += gl_TessCoord.y * te_in[1].position;
    te_out.position += gl_TessCoord.z * te_in[2].position;
    vec4 world_position = vec4(te_out.position, 1.0);

    gl_Position = projection_view * world_position;
}
