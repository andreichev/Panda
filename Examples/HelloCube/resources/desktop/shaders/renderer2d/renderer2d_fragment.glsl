#version 330 core

in float fragTextureIndex;
in vec4 fragColor;
in vec2 fragTexCoord;

out vec4 outColor;

uniform sampler2D u_textures[16];

void main() {
    // if (fragTextureIndex == 0) {
    //     outColor = fragColor;
    // } else {
    //     outColor = fragColor * texture(u_textures[int(fragTextureIndex)], fragTexCoord.st);
    // }
    outColor = fragColor * texture(u_textures[1], fragTexCoord.st);
}