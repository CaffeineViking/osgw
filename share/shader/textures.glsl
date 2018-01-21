#version 410

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D opacity_map;
uniform sampler2D normal_map;
uniform sampler2D gloss_map;

uniform sampler2D light_map;
uniform samplerCube environment_map;
