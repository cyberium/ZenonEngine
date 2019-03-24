#include "..\\_gamedata\\shaders_D3D\\CommonTypes.h"

struct Textured_Material
{
    //--------------------------- ( 0 bytes )
}; 

struct VertexShaderInput
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 positionVS : SV_POSITION;
	float4 positionWS : POSITION;
	float2 texCoord   : TEXCOORD0;
};

cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}
cbuffer Material : register(b2)
{
    Textured_Material Mat;
};

Texture2D DiffuseTexture : register(t0);
sampler DiffuseTextureSampler : register(s0);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.positionVS = mul(ModelViewProjection, float4(IN.position, 1.0f));
	OUT.positionWS = float4(IN.position, 1.0f);
	OUT.texCoord = IN.texCoord;

	return OUT;
}

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
PixelShaderOutput OUT;
	OUT.PositionWS = IN.positionWS;
	OUT.Diffuse = DiffuseTexture.Sample(DiffuseTextureSampler, IN.texCoord);
	OUT.Specular = float4(1.0, 1.0, 1.0, 1.0);
	OUT.NormalWS = float4(1.0, 1.0, 1.0, 0.0);
	return OUT;
}