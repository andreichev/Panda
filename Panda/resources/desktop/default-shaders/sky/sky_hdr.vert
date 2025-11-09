#version 330 core

layout(std140) uniform type_UBO_VERT {
    mat4 projViewMtx;
    mat4 model;
} mvp;

layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

void main() {
    gl_Position = mvp.projViewMtx * mvp.model * vec4(aPos, 1.0);
    TexCoord = aPos;
}
