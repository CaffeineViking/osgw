#version 410

// Fragment shader:

in PipelineData {
    vec3 position;
    vec3 normal;
    vec2 texture_coordinate;
} fs_in;

#pragma include("lighting.glsl")
#pragma include("textures.glsl")
#pragma include("matrices.glsl")

out vec4 color;

void main() {
    float surface_shininess = 1.0;
    vec3 diffuse_albedo  = vec3(1.0, 1.0, 1.0),
         specular_albedo = vec3(1.0, 1.0, 1.0);
    color = vec4(blinn_phong(fs_in.position, fs_in.normal,
                             diffuse_albedo, specular_albedo,
                             surface_shininess, eye_position),
            1.0f);
}
