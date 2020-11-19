#include "CommonInclude.hlsl"

cbuffer Material : register(b2)
{
    float4 DiffuseColor;
	bool   HasDiffuseTexture;
};

struct VSOut
{
	float4 position   : SV_POSITION;
	float4 positionVS : POSITION;
	float2 texCoord   : TEXCOORD0;
};

Texture2D DiffuseTexture : register(t0);

VSOut VS_main(VSInputPT IN)
{
	const float4x4 mvp = mul(PF.Projection, mul(PF.View, PO.Model));

	VSOut OUT;
	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.positionVS = float4(IN.position, 1.0f);
	OUT.texCoord = IN.texCoord;
	return OUT;
}

float4 PS_main(VSOut IN) : SV_TARGET
{
	float4 resultColor = DiffuseColor;
	if (HasDiffuseTexture)
		resultColor *= DiffuseTexture.Sample(LinearClampSampler, float2(IN.texCoord.x, 1.0f - IN.texCoord.y));
	
    return resultColor;
}