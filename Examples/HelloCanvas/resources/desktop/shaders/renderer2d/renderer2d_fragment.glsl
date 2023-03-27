#version 330 core

in float fragTextureIndex;
in vec4 fragColor;
in vec2 fragTexCoord;

out vec4 outColor;

uniform sampler2D u_textures[16];

void main() {
    outColor = fragColor;
    // * texture(u_textures[int(fragTextureIndex)], fragTexCoord.st);
    switch(int(fragTextureIndex)) {
		case  0: outColor *= texture(u_textures[ 0], fragTexCoord.st); break;
		case  1: outColor *= texture(u_textures[ 1], fragTexCoord.st); break;
		case  2: outColor *= texture(u_textures[ 2], fragTexCoord.st); break;
        case  3: outColor *= texture(u_textures[ 3], fragTexCoord.st); break;
        case  4: outColor *= texture(u_textures[ 4], fragTexCoord.st); break;
        case  5: outColor *= texture(u_textures[ 5], fragTexCoord.st); break;
        case  6: outColor *= texture(u_textures[ 6], fragTexCoord.st); break;
        case  7: outColor *= texture(u_textures[ 7], fragTexCoord.st); break;
        case  8: outColor *= texture(u_textures[ 8], fragTexCoord.st); break;
        case  9: outColor *= texture(u_textures[ 9], fragTexCoord.st); break;
        case 10: outColor *= texture(u_textures[10], fragTexCoord.st); break;
        case 11: outColor *= texture(u_textures[11], fragTexCoord.st); break;
        case 12: outColor *= texture(u_textures[12], fragTexCoord.st); break;
        case 13: outColor *= texture(u_textures[13], fragTexCoord.st); break;
        case 14: outColor *= texture(u_textures[14], fragTexCoord.st); break;
        case 15: outColor *= texture(u_textures[15], fragTexCoord.st); break;
    }
}