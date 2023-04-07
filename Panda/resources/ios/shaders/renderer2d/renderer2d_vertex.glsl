#version 300 es

precision highp float;

layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 Color;

uniform mat4 ProjViewMtx;

out vec4 Frag_Color;

void main(){
    Frag_Color = Color;
    gl_Position = ProjViewMtx * vec4(Position, 1);
}
