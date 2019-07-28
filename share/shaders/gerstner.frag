#version 410

// Fragment shader: now that the ocean is being animated with Gerstner waves, the only
// part that is left to do is shade it. Here we use a simple Blinn-Phong shading model
// that takes two base colors (ones for shallow waves and another for deep waves), and
// combines them relative to the height of the wave. After that, we apply some surface
// ripples, to increase the details of the waves without having to add extra geometry.
// This will modify the normal of the wave, so we need to re-calculate it (again yes).
// Finally, we also apply the skybox reflection on the water. We mix these components.

in PipelineData {
    vec3 position;
    vec3 normal;
    vec2 texture_coordinate;
} fs_in;

#pragma include("lighting.glsl")
#pragma include("textures.glsl")
#pragma include("matrices.glsl")
#pragma include("snoise3d.glsl")

out vec4 color;

void main() {
    // Primary colors of these waves (not accurate!).
    vec3 deep_wave_albedo    = vec3(0, 0.188, 0.203),
         shallow_wave_albedo = vec3(0.152, 1, 0.737);

    float limit_wave_height = 1.28; // This determines shallow/deep water.
    // First we decide how "deep" the wave is. This might not make much of
    // physical sense, but it gives a nice effect. We base it on the waves
    // height, and lerp the shallow and deep reflectivities based on that.
    float wave_depth = smoothstep(-limit_wave_height, +limit_wave_height,
                                  fs_in.position.y);
    vec3 diffuse_wave_albedo = mix(deep_wave_albedo,
                                   shallow_wave_albedo,
                                   wave_depth);

    // To give the effect of small wave ripples, we create
    // a procedural simplex noise function with 2 octaves.
    vec3 ripd = 0.5*time * vec3(cos(0.32), 1.0, sin(0.32));
    vec3 wave_ripple_normal, wave_ripple_norm_1, wave_ripple_norm_2;
    float wave_ripple = snoise3d(0.5*fs_in.position+ripd, wave_ripple_norm_1);
    wave_ripple += 0.5*snoise3d(4*fs_in.position+ripd/2,wave_ripple_norm_2);
    // // We also calculate the normal of ripple, so we can change the normals.
    wave_ripple_normal = normalize(wave_ripple_norm_1 + wave_ripple_norm_2);
    vec3 wave_ripple_color = vec3((wave_ripple + 1.5) / 3.0); // and colors.

    // We use the value from them to darken or brighten certain spots on it.
    vec3 diffuse_albedo = mix(diffuse_wave_albedo, wave_ripple_color, 0.10);

    // Finally, we apply the shading and mix it with the fog in the scene.
    float fog = clamp(pow(distance(look_at_point, fs_in.position) / 24, 1), 0, 1);
    vec3 shading_color = blinn_phong(fs_in.position, fs_in.normal,
                                     diffuse_albedo, vec3(0.125f),
                                     1.0, eye_position);
    color = vec4(mix(shading_color, fog_color, fog), 1);
}
