#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 Pos;
in vec4 Color;

out vec4 FragColor;

uniform sampler2D texture1;
uniform samplerCube iSky;
uniform vec4 cameraPos;

void main()
{
	vec3 I = normalize(Pos - cameraPos.xyz);
	vec3 R = reflect(I, normalize(Normal));
	vec4 sky = texture(iSky, R);
	FragColor = mix(sky, texture(texture1, TexCoord), 0.4) * 1.2;
}
