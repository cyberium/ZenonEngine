#include "CommonInclude.hlsl"

struct VSOut
{
	float4 position   : SV_POSITION;
	float3 positionWS : POSITION;
};

TextureCube SkyboxCubeTexture : register(t0);

VSOut VS_Skybox(VSInputP IN)
{
	const float4x4 mvp = mul(PF.Projection, mul(PF.View, PO.Model));

	VSOut OUT;
	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.positionWS = IN.position;
	return OUT;
}

float4 PS_Skybox(VSOut IN) : SV_TARGET
{
    return SkyboxCubeTexture.Sample(LinearClampSampler, IN.positionWS);
}