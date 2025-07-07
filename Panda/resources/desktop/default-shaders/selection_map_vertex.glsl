#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in int textureIndex;
layout (location = 3) in vec4 color;
layout (location = 4) in uint id;

uniform mat4 projViewMtx;

void main(){
    gl_Position = projViewMtx * vec4(position, 1);
}
