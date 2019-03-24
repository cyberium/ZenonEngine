#version 330

// Input
in struct
{
	vec3 WorldSpacePos;
	vec4 VertexColorMCCV;
} VSInput;

out vec3 color;

void main()
{	
	color = VSInput.VertexColorMCCV.rgb;
};
