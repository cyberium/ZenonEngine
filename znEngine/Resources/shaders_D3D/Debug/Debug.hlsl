#include "..\\_gamedata\\shaders_D3D\\CommonTypes.h"

struct Debug_Material
{
    float4 DiffuseColor;
};

struct VertexShaderInput
{
	float3 position       : POSITION;
};
struct VertexShaderOutput
{
	float4 positionVS     : SV_POSITION;
	float4 positionWS     : POSITION;
};

// Uniforms
cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}
cbuffer Material : register(b2)
{
    Debug_Material Material;
};

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.positionVS = mul(ModelViewProjection, float4(IN.position, 1.0f));
	OUT.positionWS = float4(IN.position, 1.0f);
	return OUT;
}

PixelShaderOutput PS_main(VertexShaderOutput IN) : SV_TARGET
{
	PixelShaderOutput OUT;
	OUT.PositionWS = IN.positionWS;
	OUT.Diffuse = Material.DiffuseColor;
	OUT.Specular = float4(1.0, 1.0, 1.0, 1.0);
	OUT.NormalWS = float4(1.0, 1.0, 1.0, 0.0);
	return OUT;
}