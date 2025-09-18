#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec4 color;
layout (location = 3) in uint id;

layout(std140) uniform UBO_VERT {
    mat4 projViewMtx;
    mat4 modelMtx;
} ubo_vert;

out vec4 fragColor;
out vec2 fragTexCoord;
flat out uint fragId;

void main(){
    gl_Position = ubo_vert.projViewMtx * ubo_vert.modelMtx * vec4(position, 1);
    fragTexCoord = texCoord;
    fragColor = color;
    fragId = id;
}
