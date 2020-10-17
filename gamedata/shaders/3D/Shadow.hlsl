#include "CommonInclude.hlsl"

struct VS_Output
{
	float4 position : SV_POSITION;
	float depthPosition : DEPTH;
};

VS_Output VS_Shadow(VSInputP IN)
{
	const float4x4 mvl = mul(PF.View, PO.Model);
	const float4x4 mvpl = mul(PF.Projection, mvl);

	VS_Output VSOut;
	VSOut.position = mul(mvpl, float4(IN.position.xyz, 1.0f));
	VSOut.depthPosition = (VSOut.position.z / VSOut.position.w) ;
	return VSOut;
}

float4 PS_Shadow(VS_Output VSOut) : SV_TARGET
{
	return float4(VSOut.depthPosition, 0.0f, 0.0f, 0.0f);
}