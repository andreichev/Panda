#version 330 core
#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
in vec4 Color;

out vec4 FragColor;
#else
varying vec4 Color;
#endif

void main()
{
    #if __VERSION__ >= 140
    FragColor = Color;
    #else
    gl_FragColor = Color;
    #endif
}
