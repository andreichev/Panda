#version 330

layout(std140) uniform UniformBuffer {
    float iTime;
    vec2 iResolution;
} ub;

layout(location = 0) out vec4 fragColor;
in vec4 Color;

void main()
{
    vec2 uv = gl_FragCoord.xy / ub.iResolution;
    vec3 col = vec3(0.5) + (cos((vec3(ub.iTime) + uv.xyx) + vec3(0.0, 2.0, 4.0)) * 0.5);
    fragColor = vec4(col, 1.0);
}

