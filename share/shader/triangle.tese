#version 410

layout(triangles) in;

in PipelineData {
    vec4 position;
    vec2 texture_coordinate;
} te_in[];

out PipelineData {
    vec4 position;
    vec2 texture_coordinate;
} te_out;

void main() {
    te_out.texture_coordinate  = gl_TessCoord.x * te_in[0].texture_coordinate;
    te_out.texture_coordinate += gl_TessCoord.y * te_in[1].texture_coordinate;
    te_out.texture_coordinate += gl_TessCoord.z * te_in[2].texture_coordinate;

    te_out.position  = gl_TessCoord.x * te_in[0].position;
    te_out.position += gl_TessCoord.y * te_in[1].position;
    te_out.position += gl_TessCoord.z * te_in[2].position;

    gl_Position = te_out.position;
}
