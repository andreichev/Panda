#version 330

in vec2 fragTexCoord;

uniform vec4 hoveredId;
uniform usampler2D highlightMapTexture;
uniform sampler2D colorTexture;

out vec4 colorOut;

void main() {
    uint id = uint(texture(highlightMapTexture, fragTexCoord).r);
    if (id == 0u) {
        colorOut = texture(colorTexture, fragTexCoord);
    } else {
        colorOut = vec4(1.0, 0.0, 0.0, 1.0);
    }
}