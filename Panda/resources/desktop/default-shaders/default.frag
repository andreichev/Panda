#version 330 core

in vec4 fragColor;
in vec2 fragTexCoord;
flat in uint fragId;

layout(location = 0) out vec4 outColor;
layout(location = 1) out uint outId;

uniform sampler2D albedo;

void main() {
    outColor = fragColor;
    outColor *= texture(albedo, fragTexCoord.st);
    if (outColor.a <= 0.01) {
        discard;
    }
    outId = fragId;
}