#version 330 core

in vec4 fragColor;
in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

uniform sampler2D u_texture;

void main() {
    outColor = fragColor;
    outColor *= texture(u_texture, fragTexCoord.st);
    if (outColor.a <= 0.01) {
        discard;
    }
}