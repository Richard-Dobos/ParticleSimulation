#version 440 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in int a_Color;

out vec4 color;

void main()
{
	color = vec4(
					float(a_Color >> 24 & 0xFF) / 255,
					float(a_Color >> 16 & 0xFF) / 255,
					float(a_Color >> 8 & 0xFF) / 255,
					float(a_Color & 0xFF) / 255
					);
	gl_Position = vec4(a_Pos, 1.0);
}