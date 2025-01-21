#version 330 core

in vec3 TexCoord;

uniform sampler2D skyTexture;

layout(location = 0) out vec4 outColor;
layout(location = 1) out uint outId;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 sampleSphericalMap(vec3 v)  {
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}

void main() {
	vec2 uv = sampleSphericalMap(normalize(vec3(TexCoord.x, -TexCoord.y, TexCoord.z)));
	vec3 color = texture(skyTexture, uv).rgb;

	outColor = vec4(color, 1.0);
	outId = 0;
}