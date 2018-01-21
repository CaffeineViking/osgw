#version 410

in PipelineData {
    vec3 position;
    vec3 normal;
    vec2 texture_coordinate;
} fs_in;

uniform float time;
uniform sampler2D megumin_texture;

out vec4 color;

void main() {
    vec4 megumin_texel = texture(megumin_texture, fs_in.texture_coordinate);
    if (megumin_texel.r < 0.2 &&
        megumin_texel.g < 0.2 &&
        megumin_texel.b < 0.2) discard;

    vec3 thresholds = vec3(sin(time), cos(time), sin(time));
    vec3 disco_texel = mix(megumin_texel.rgb, fs_in.position, thresholds);
    color = vec4(disco_texel, 1.0); // Blend b.w texture and the position.
}
