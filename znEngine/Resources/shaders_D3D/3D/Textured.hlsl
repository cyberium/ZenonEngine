#include "IDB_SHADER_COMMON_TYPES"

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
	float4x4 Model;
	float4x4 View;
	float4x4 Projection;
}
cbuffer Material : register(b2)
{
    Textured_Material Mat;
};

Texture2D DiffuseTexture : register(t0);
sampler DiffuseTextureSampler : register(s0);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	const float4x4 mvp = mul(Projection, mul(View, Model));

	VertexShaderOutput OUT;
	OUT.positionVS = mul(mvp, float4(IN.position, 1.0f));
	OUT.positionWS = float4(IN.position, 1.0f);
	OUT.texCoord = IN.texCoord;

	return OUT;
}

PixelShaderOutput PS_main(VertexShaderOutput IN) : SV_TARGET
{
	float4 diffuseColor = DiffuseTexture.Sample(DiffuseTextureSampler, IN.texCoord);
	if (diffuseColor.a < 0.05f)
		discard;

    PixelShaderOutput OUT;
	OUT.PositionWS = IN.positionWS;
	OUT.Diffuse = diffuseColor;
	OUT.Specular = float4(1.0, 1.0, 1.0, 1.0);
	OUT.NormalWS = float4(1.0, 1.0, 1.0, 0.0);
	return OUT;
}