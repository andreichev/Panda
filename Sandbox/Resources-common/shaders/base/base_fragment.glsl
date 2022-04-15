// #version 330 core
#version 300 es
#ifdef GL_ES
precision highp float;
#endif

in vec2 TexCoord;
in vec4 Color;

out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
	FragColor = Color * texture(texture1, TexCoord);
}
