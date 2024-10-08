#version 330 core

uniform mat4 projViewMtx;
float gridSize = 100.0;
uniform vec4 gCameraWorldPos;

layout (location = 0) in vec2 aPos;

out vec3 WorldPos;

void main() {
    vec4 pos = vec4(aPos.x * gridSize, 0.0, aPos.y * gridSize, 1.0);
    pos.x += gCameraWorldPos.x;
    pos.z += gCameraWorldPos.z;
    gl_Position = projViewMtx * pos;
    WorldPos = pos.xyz;
}
