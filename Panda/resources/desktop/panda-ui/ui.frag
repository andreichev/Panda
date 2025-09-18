#version 330 core

in vec4 fragColor;
in vec2 fragTexCoord;
flat in uint fragId;

layout(location = 0) out vec4 outColor;
layout(location = 1) out uint outId;

uniform sampler2D u_texture;

void main() {
    outColor = fragColor;
    outColor *= texture(u_texture, fragTexCoord.st);
    if (outColor.a <= 0.01) {
        discard;
    }
    outId = fragId;
}