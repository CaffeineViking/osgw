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
#pragma include("snoise3d.glsl")

out PipelineData {
    vec3 position;
    vec3 normal;
    vec2 texture_coordinate;
} te_out;

void main() {
    // Below we just interpolate the position, normal, and texture coordinates by
    // using the weights of the new vertices produced from the tessellation unit.
    vec3 x_up_normal_mix = mix(te_in[0].normal, te_in[3].normal, gl_TessCoord.x);
    vec3 x_down_normal_mix = mix(te_in[1].normal, te_in[2].normal, gl_TessCoord.x);
    te_out.normal = mix(x_down_normal_mix, x_up_normal_mix, gl_TessCoord.y);

    vec2 x_up_texture_mix = mix(te_in[0].texture_coordinate, te_in[3].texture_coordinate, gl_TessCoord.x);
    vec2 x_down_texture_mix = mix(te_in[1].texture_coordinate, te_in[2].texture_coordinate, gl_TessCoord.x);
    te_out.texture_coordinate = mix(x_down_texture_mix, x_up_texture_mix, gl_TessCoord.y);

    vec3 x_up_position_mix = mix(te_in[0].position, te_in[3].position, gl_TessCoord.x);
    vec3 x_down_position_mix = mix(te_in[1].position, te_in[2].position, gl_TessCoord.x);
    te_out.position = mix(x_down_position_mix, x_up_position_mix, gl_TessCoord.y);
    float distance_from_center = distance(look_at_point.xz,te_out.position.xz)/24;

    // Displace the tessellated geometry in the direction of the normal by us-
    // ing a sum of Gerstner waves. A effect of this wave is that it will move
    // vertices closer to the waves' crest (the tallest peak) depending on the
    // steepness parameter. The height of the wave is done by the sum of sines
    // method. The parameters can be changed in the shader or by the uniforms.
    te_out.position = gerstner_wave(te_out.position.xz, time,  te_out.normal);

    vec3 normal; // Below we add a bit more detail by adding 3D Simplex Noise.
    te_out.position.y += 0.40*snoise3d(0.2*te_out.position+0.40*time, normal);
    te_out.position.y += 0.20*snoise3d(0.4*te_out.position+0.20*time, normal);
    te_out.position.y += 0.10*snoise3d(0.8*te_out.position+0.10*time, normal);
    te_out.position.y += 0.05*snoise3d(1.6*te_out.position+0.05*time, normal);
    te_out.position.y += 0.02*snoise3d(3.2*te_out.position+0.02*time, normal);

    vec4 world_position = vec4(te_out.position, 1);
    gl_Position = projection_view * world_position;
}
