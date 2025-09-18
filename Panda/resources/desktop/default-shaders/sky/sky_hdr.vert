#version 330 core

layout(std140) uniform MVP {
    mat4 model;
    mat4 projViewMtx;
} mvp;

layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

void main() {
    gl_Position = mvp.projViewMtx * mvp.model * vec4(aPos, 1.0);
    TexCoord = aPos;
}
