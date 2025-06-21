#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in int textureIndex;
layout (location = 3) in vec4 color;
layout (location = 4) in uint id;

uniform mat4 projViewMtx;

flat out int fragTextureIndex;
out vec4 fragColor;
out vec2 fragTexCoord;
flat out uint fragId;

void main(){
    gl_Position = projViewMtx * vec4(position, 1);
    fragTexCoord = texCoord;
    fragTextureIndex = textureIndex;
    fragColor = color;
    fragId = id;
}
