#version 330 core
#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aLight;

out vec4 Color;
out vec2 TexCoord;
#else
attribute vec3 aPos;
attribute vec2 aTexCoord;
attribute float aLight;

varying vec4 Color;
varying vec2 TexCoord;
#endif

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	Color = vec4(aLight, aLight, aLight, 1.0);
	TexCoord = aTexCoord;
}
