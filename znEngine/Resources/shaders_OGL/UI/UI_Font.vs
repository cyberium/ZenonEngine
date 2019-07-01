#version 440
#extension GL_ARB_explicit_uniform_location : enable

// Vertex attrib
layout(location = 0) in vec2 POSITION;
layout(location = 1) in vec2 TEXCOORD0;

// Output
out gl_PerVertex
{
    vec4 gl_Position;
};
out struct
{
	vec4 POSITION;
	vec2 TEXCOORD0;
} VSInput;

// Uniforms
uniform PerObject 
{
    mat4 ModelViewProjection;
};

// Uniforms
uniform Material 
{
    vec4 Color;
	vec2 Offset;
};

void main(void)
{
	vec4 ResultPosition = vec4(POSITION, 0.0, 1.0) + vec4(Offset, 0.0, 0.0f);

	gl_Position = ModelViewProjection * ResultPosition;

	VSInput.POSITION  = ResultPosition;
	VSInput.TEXCOORD0 = TEXCOORD0;
};