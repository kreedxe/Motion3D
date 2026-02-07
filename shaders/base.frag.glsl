#version 460 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D ourTexture;
uniform vec3 lightColor;

void main()
{
   fragColor = texture(ourTexture, texCoord) * lightColor;
}
