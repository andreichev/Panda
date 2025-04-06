#version 300 es

precision highp float;

uniform mat4 model;
uniform mat4 projViewMtx;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec4 aColor;
layout (location = 4) in float aLight;

out vec4 Color;
out vec2 TexCoord;
out vec3 Normal;
out vec3 Pos;

void main()
{
    gl_Position = projViewMtx * model * vec4(aPos, 1.0);
    Color = vec4(aColor.r * aLight, aColor.g * aLight, aColor.b * aLight, aColor.a);
    TexCoord = aTexCoord;
    Normal = aNormal;
    Pos = aPos;
}
