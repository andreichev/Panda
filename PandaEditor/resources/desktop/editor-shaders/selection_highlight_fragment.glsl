#version 330

in vec2 fragTexCoord;

uniform float time;
uniform vec2 resolution;
uniform sampler2D colorTexture;
uniform usampler2D isSelectedTexture;

out vec4 colorOut;

vec4 getOutlineColor() {
    // vec2 uv = gl_FragCoord.xy/resolution.xy;
    // vec3 col = 0.5 + 0.5 * cos(time+uv.xyx+vec3(0,2,4));
    // return vec4(col, 1.0);
    return vec4(0.92, 0.55, 0.0, 1.0);
}

bool isSelected() {
    ivec2 texelCoord = ivec2(gl_FragCoord.xy);
    uint isSelected = texelFetch(isSelectedTexture, texelCoord, 0).r;
    return isSelected != 0u;
}

vec4 getSelectionColor() {
    return isSelected() ? vec4(1.0) : vec4(0.0);
}

float getSelectionBlurredColor() {
    float Pi2 = 6.28318530718;
    float directions = 8.0;
    float quality = 3.0;
    // float size = 8.0 + (0.5 + 0.5 * cos(time * 3.0)) * 4.0;
    float size = 8.0;
    vec2 radius = size/resolution.xy;
    vec2 uv = gl_FragCoord.xy/resolution.xy;
    float color = texture(isSelectedTexture, uv).r;
    for( float d=0.0; d<Pi2; d+=Pi2/directions) {
        for(float i=1.0/quality; i<=1.0; i+=1.0/quality) {
            color += texture(isSelectedTexture, uv+vec2(cos(d),sin(d))*radius*i).r;
        }
    }
    color /= quality * directions - 15.0;
    return color;
}

void main() {
    vec4 color = texture(colorTexture, fragTexCoord);
    vec4 outline = getOutlineColor();
    outline.a = isSelected() ? 0.0 : getSelectionBlurredColor();
    colorOut = mix(color, outline, outline.a);
    colorOut.a = 1.0;
}