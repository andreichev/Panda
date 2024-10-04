#version 330 core

in vec3 TexCoord;

layout(location = 0) out vec4 outColor;
layout(location = 1) out int outId;

uniform samplerCube skyTexture;

void main()
{
	outColor = texture(skyTexture, TexCoord);
	outId = -1;
}
