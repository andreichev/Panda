#version 330 core

in float fragTextureIndex;
in vec4 fragColor;
in vec2 fragTexCoord;
in float fragId;

out vec4 outColor;

void main() {
    outColor = vec4(fragId, 0.0, 0.0, 1.0);
}