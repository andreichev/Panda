#version 330

in vec2 fragTexCoord;

uniform vec4 hoveredId;
uniform sampler2D colorTexture;

out vec4 colorOut;

void main() {
    vec4 color = texture(colorTexture, fragTexCoord);
    color.r = 1.0;
    colorOut = color;
}