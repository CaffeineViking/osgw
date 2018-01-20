#version 410

in PipelineData {
    vec4 position;
    vec2 texture_coordinate;
} fs_in;

uniform float time;
uniform sampler2D megumin_texture;

out vec4 color;

void main() {
    vec4 megumin_texel = texture(megumin_texture, fs_in.texture_coordinate);
    if (megumin_texel.r < 0.2 && megumin_texel.g < 0.2 && megumin_texel.b < 0.2)
        discard;

    vec3 disco_mixer = vec3(sin(time), cos(time), sin(time));
    vec3 mixed_texel = mix(megumin_texel.rgb, fs_in.position.xyz, disco_mixer);
    color = vec4(mixed_texel, 1.0);
}
