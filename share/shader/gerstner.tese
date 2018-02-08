#version 410

layout(quads, fractional_even_spacing) in;

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
    vec3 x_up_normal_mix = mix(te_in[0].normal, te_in[3].normal, gl_TessCoord.x);
    vec3 x_down_normal_mix = mix(te_in[1].normal, te_in[2].normal, gl_TessCoord.x);
    te_out.normal = mix(x_down_normal_mix, x_up_normal_mix, gl_TessCoord.y);

    vec2 x_up_texture_mix = mix(te_in[0].texture_coordinate, te_in[3].texture_coordinate, gl_TessCoord.x);
    vec2 x_down_texture_mix = mix(te_in[1].texture_coordinate, te_in[2].texture_coordinate, gl_TessCoord.x);
    te_out.texture_coordinate = mix(x_down_texture_mix, x_up_texture_mix, gl_TessCoord.y);

    vec3 x_up_position_mix = mix(te_in[0].position, te_in[3].position, gl_TessCoord.x);
    vec3 x_down_position_mix = mix(te_in[1].position, te_in[2].position, gl_TessCoord.x);
    te_out.position = mix(x_down_position_mix, x_up_position_mix, gl_TessCoord.y);
    vec4 world_position = vec4(te_out.position, 1.0);

    gl_Position = projection_view * world_position;
}
