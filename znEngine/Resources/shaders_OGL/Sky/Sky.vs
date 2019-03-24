#version 330

// Vertex attrib
layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec4 COLOR0;

// Output
out struct
{
	vec3 POSITION;
	vec4 COLOR0;
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
	VSInput.COLOR0   = COLOR0;
};