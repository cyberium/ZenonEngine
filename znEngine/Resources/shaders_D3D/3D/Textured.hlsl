#include "IDB_SHADER_COMMON_TYPES"

struct VertexShaderOutput
{
	float4 positionVS : SV_POSITION;
	float4 positionWS : POSITION;
	float2 texCoord   : TEXCOORD0;
};

cbuffer PerObject : register(b0)
{
	PerObject PO;
}
cbuffer Material : register(b2)
{};

Texture2D DiffuseTexture : register(t0);
sampler DiffuseTextureSampler : register(s0);

VertexShaderOutput VS_main(VSInputPT IN)
{
	const float4x4 mvp = mul(PO.Projection, mul(PO.View, PO.Model));

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