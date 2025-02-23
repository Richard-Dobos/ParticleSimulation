#version 440 core

in vec4 color;

out vec4 FragColor;

void main()
{
	FragColor = color;
	//FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}