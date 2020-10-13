
struct PerObject
{
	float4x4 Model;
};

struct PerFrame
{
	float4x4 View;
	float4x4 Projection;
};

cbuffer PerObject : register(b0)
{
	PerObject PO;
}

cbuffer PerFrame : register(b1)
{
	PerFrame PF;
}

struct VertexShaderInput
{
	float2 position : POSITION;
    float2 texCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};