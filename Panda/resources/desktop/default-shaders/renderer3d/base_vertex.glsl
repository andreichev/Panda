#version 330 core

uniform mat4 model;
uniform mat4 projViewMtx;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aLight;

out vec4 Color;
out vec2 TexCoord;

void main()
{
    gl_Position = projViewMtx * model * vec4(aPos, 1.0);
    Color = vec4(aLight, aLight, aLight, 1.0);
    TexCoord = aTexCoord;
}
