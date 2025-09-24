#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec4 color;

layout(std140) uniform UBO {
    mat4 projViewMtx;
} ubo;

out vec4 fragColor;
out vec2 fragTexCoord;

void main(){
    gl_Position = ubo.projViewMtx * vec4(position, 1);
    fragTexCoord = texCoord;
    fragColor = color;
}
