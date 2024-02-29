#version 440 core

layout(location = 0) in vec3 in_Pos;
layout(location = 1) in int in_Color;

out vec4 out_Color;
out vec3 out_Pos;

void main()
{
	out_Color = vec4(float((in_Color >> 24) & 0xFF) / 255.0, float((in_Color >> 16) & 0xFF) / 255.0, float((in_Color >> 8) & 0xFF) / 255.0, float(in_Color & 0xFF) / 255.0);
	out_Pos = in_Pos;
	gl_Position = vec4(in_Pos, 1.0);
}