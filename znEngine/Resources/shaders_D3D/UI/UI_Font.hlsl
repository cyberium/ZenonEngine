#include "IDB_SHADER_UI_COMMON"

cbuffer Material : register(b2)
{
    float4 Color;
    float2 Offset;
};

Texture2D DiffuseTexture : register(t0);
sampler DiffuseTextureSampler : register(s0);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	const float4 resultPos = float4(IN.position, 0.0f, 1.0f) + float4(Offset, 0.0f, 0.0f);
	
	const float4x4 mp = mul(PF.Projection, PO.Model);

	VertexShaderOutput OUT;
	OUT.position = mul(mp, resultPos);
	OUT.texCoord = IN.texCoord;
	return OUT;
}

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
    return float4(Color.rgb, Color.a * DiffuseTexture.Sample(DiffuseTextureSampler, IN.texCoord).a);
}