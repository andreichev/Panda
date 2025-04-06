#version 300 es

precision highp float;

in vec2 TexCoord;
in vec3 Normal;
in vec3 Pos;
in vec4 Color;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D iSky;
uniform vec4 cameraPos;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 sampleSphericalMap(vec3 v)  {
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}

void main() {
	vec3 I = normalize(Pos - cameraPos.xyz);
	vec3 R = reflect(I, normalize(Normal));

	vec2 uv = sampleSphericalMap(R);
	vec4 sky = texture(iSky, uv);
	FragColor = mix(sky, texture(texture1, TexCoord), 0.4) * 1.2;
}
