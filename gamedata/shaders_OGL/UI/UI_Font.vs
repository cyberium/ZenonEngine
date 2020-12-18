#version 440
#extension GL_ARB_explicit_uniform_location : enable

// Vertex attrib
layout(location = 0) in vec2 POSITION0;
layout(location = 1) in vec2 TEXCOORD0;

// Output
out gl_PerVertex
{
    vec4 gl_Position;
};
out struct
{
	vec4 POSITION;
	vec2 TEXCOORD;
} VSOutput;

layout(std140, binding = 0) uniform PerObject 
{
    mat4 ModelViewProjection;
};

layout(std140, binding = 1) uniform Material 
{
    vec4 Color;
	vec2 Offset;
	vec2 Unused;
};

void main(void)
{
	vec4 ResultPosition = vec4(POSITION0, 0.0, 1.0)+ vec4(Offset, 0.0, 0.0f);

	gl_Position = ModelViewProjection * ResultPosition;

	VSOutput.POSITION  = ResultPosition;
	VSOutput.TEXCOORD = TEXCOORD0;
};