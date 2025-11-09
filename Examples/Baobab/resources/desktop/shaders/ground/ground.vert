#version 330 core

layout(std140) uniform type_UBO_VERT {
    mat4 projViewMtx;
    mat4 modelMtx;
} ubo_vert;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;
layout (location = 3) in float aLight;

out vec4 Color;
out vec2 TexCoord;

void main()
{
    gl_Position = ubo_vert.projViewMtx * ubo_vert.modelMtx * vec4(aPos, 1.0);
    Color = vec4(aColor.r * aLight, aColor.g * aLight, aColor.b * aLight, aColor.a);
    TexCoord = aTexCoord;
}
