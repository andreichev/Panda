#version 330 core

flat in int fragTextureIndex;
in vec4 fragColor;
in vec2 fragTexCoord;
flat in uint fragId;

layout(location = 0) out vec4 outColor;
layout(location = 1) out uint outId;

uniform sampler2D u_textures[8];

void main() {
    outColor = fragColor;
    // outColor = texture(u_textures[fragTextureIndex], fragTexCoord.st);
    switch(fragTextureIndex) {
		case  0: outColor *= texture(u_textures[ 0], fragTexCoord.st); break;
		case  1: outColor *= texture(u_textures[ 1], fragTexCoord.st); break;
		case  2: outColor *= texture(u_textures[ 2], fragTexCoord.st); break;
        case  3: outColor *= texture(u_textures[ 3], fragTexCoord.st); break;
        case  4: outColor *= texture(u_textures[ 4], fragTexCoord.st); break;
        case  5: outColor *= texture(u_textures[ 5], fragTexCoord.st); break;
        case  6: outColor *= texture(u_textures[ 6], fragTexCoord.st); break;
        case  7: outColor *= texture(u_textures[ 7], fragTexCoord.st); break;
    }
    if (outColor.a <= 0.01) {
        discard;
    }
    outId = fragId;
}