#version 330 core

layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec4 Color;

layout(std140) uniform UBO {
    mat4 ProjMtx;
} ubo;

out vec2 Frag_UV;
out vec4 Frag_Color;

void main() {
    Frag_UV = UV;
    Frag_Color = Color;
    gl_Position = ubo.ProjMtx * vec4(Position.xy, 0, 1);
}