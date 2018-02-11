#version 410

// Texture samplers: in order to keep things simple and elegant, we assume that
// the shader program can use (if it wants) the following texture samplers. Any
// other textures will not be automatically used by the light reflection model.

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D opacity_map;
uniform sampler2D normal_map;

uniform sampler2D light_map;
uniform samplerCube reflection_map;
