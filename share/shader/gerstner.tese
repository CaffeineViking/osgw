#version 410

// Tessellation evaluation shader: after tessellation has been done, new vertices have
// been generated. The tess. evaluation shaders purpose is to decide on where to place
// these new vertices. In our case, all of the vertex properties (position and normal)
// are interpolated bilinearly (for a quad) to find the new vertex properties. And, we
// also displace the new vertex according to the Gerstner wave, to produce a realistic
// ocean surface animation (varying by using the vertices X-Z coordinate and the time).
// We also do world --> projection transformation here, since we couldn't do it before.

layout(quads, fractional_even_spacing) in;

in PipelineData {
    vec3 position;
    vec3 normal;
    vec2 texture_coordinate;
} te_in[];

#pragma include("matrices.glsl")
#pragma include("gerstner.glsl")

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

    // Displace the tessellated geometry in the normal's direction, thus
    // we use a Gerstner wave to decide how tall the wave should be. The
    // function is general, but how we are currently using it is not, as
    // the position won't work with non-plane geometry. Just use the te-
    // xture points instead, and it'll work for e.g. spheres and models.
    te_out.position += te_out.normal * gerstner_wave(te_out.position.xz,
                                                     time); // seconds
    vec4 world_position = vec4(te_out.position, 1.0);

    gl_Position = projection_view * world_position;
}
