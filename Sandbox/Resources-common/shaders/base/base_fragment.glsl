#version 330 core
#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
in vec2 TexCoord;
in vec4 Color;

out vec4 FragColor;
#else
varying vec2 TexCoord;
varying vec4 Color;
#endif

uniform sampler2D texture1;

void main()
{
#if __VERSION__ >= 140
	FragColor = Color * texture(texture1, TexCoord);
	// FragColor = Color * vec4(1.0f, 0.5f, 0.2f, 1.0f);
#else
    gl_FragColor = Color * texture2D(texture1, TexCoord);
#endif
}
