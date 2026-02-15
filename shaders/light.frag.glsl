#version 460 core

out vec4 fragColor;

in vec3 lColor;

void main()
{
   fragColor = vec4(lColor, 1.0f);
}
