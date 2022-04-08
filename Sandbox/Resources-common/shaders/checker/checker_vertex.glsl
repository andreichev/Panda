// #version 330 core
#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
layout (location = 0) in vec2 aPos;

out vec4 Color;
#else
attribute vec2 aPos;

varying vec4 Color;
#endif

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    Color = vec4(1.0, 0.0, 0.2, 1.0);
}
