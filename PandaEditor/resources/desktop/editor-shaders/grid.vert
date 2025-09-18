#version 330 core

float gridSize = 100.0;

layout(std140) uniform UBO1 {
    vec4 gCameraWorldPos;
    mat4 projViewMtx;
} ubo1;

layout (location = 0) in vec2 aPos;

out vec3 WorldPos;

void main() {
    vec4 pos = vec4(aPos.x * gridSize, 0.0, aPos.y * gridSize, 1.0);
    pos.x += ubo1.gCameraWorldPos.x;
    pos.z += ubo1.gCameraWorldPos.z;
    gl_Position = ubo1.projViewMtx * pos;
    WorldPos = pos.xyz;
}
