#include "CommonInclude.hlsl"

struct VSOutputShadow
{
	float4 position : SV_POSITION;
	float depthPosition : DEPTH;
};

VSOutputShadow VS_Shadow(VSInputP IN)
{
	const float4x4 mvl = mul(PF.View, PO.Model);
	const float4x4 mvpl = mul(PF.Projection, mvl);

	VSOutputShadow VSOut;
	VSOut.position = mul(mvpl, float4(IN.position.xyz, 1.0f));
	VSOut.depthPosition = (VSOut.position.z / VSOut.position.w);
	return VSOut;
}

float4 PS_Shadow(VSOutputShadow VSOut) : SV_TARGET
{
	return float4(VSOut.position.z / VSOut.position.w, 0.0f, 0.0f, 0.0f);
}