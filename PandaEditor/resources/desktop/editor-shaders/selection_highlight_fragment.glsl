#version 330

in vec2 fragTexCoord;

uniform sampler2D colorTexture;
uniform sampler2D selectedGeometryTexture;

out vec4 colorOut;

vec4 getOutlineColor() {
    return vec4(0.92, 0.55, 0.0, 1.0);
}

void main() {
    vec4 color = texture(colorTexture, fragTexCoord);
    vec4 selection = texture(selectedGeometryTexture, fragTexCoord);
    colorOut = color + selection * getOutlineColor();
}