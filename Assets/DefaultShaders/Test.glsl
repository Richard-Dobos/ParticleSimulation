#version 440 core

in vec4 color;

out vec4 FragColor;

vec4 invertColor()
{
	return vec4(1 - color.xyz, 1);
}

void main()
{
	FragColor = invertColor();
}