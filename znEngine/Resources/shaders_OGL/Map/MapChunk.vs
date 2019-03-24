#version 440
#extension GL_ARB_explicit_uniform_location : enable

// Vertex attrib
layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL0;
layout(location = 2) in vec4 COLOR0;
layout(location = 3) in vec2 TEXCOORD0; // Detail
layout(location = 4) in vec2 TEXCOORD1; // Alpha

// Output
out struct
{
	vec3 POSITION;
	vec3 NORMAL0;
	vec4 COLOR0;
	vec2 TEXCOORD0;
	vec2 TEXCOORD1;
} VSInput;

// Uniforms
uniform PerObject 
{
    mat4 gProjView;
};

void main(void)
{
	gl_Position = gProjView * vec4(POSITION, 1.0);

	VSInput.POSITION = POSITION;
	VSInput.NORMAL0       = vec3(-NORMAL0.y, NORMAL0.z, -NORMAL0.x) / 127.0f; // Map chunk specific
	VSInput.COLOR0        = COLOR0;
	VSInput.TEXCOORD0     = TEXCOORD0;
	VSInput.TEXCOORD1     = TEXCOORD1;
};