#version 330 core

layout(location = 0) in vec3 position_modelspace;
layout(location = 1) in vec3 normal_modelspace;
layout(location = 2) in vec3 texCoords;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

void main(){
    mat4 worldViewProj = projection * view * world;
    gl_Position = worldViewProj * vec4(position_modelspace, 1);
}