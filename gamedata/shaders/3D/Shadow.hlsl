#include "CommonInclude.hlsl"

struct VS_Output
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};

VS_Output VS_Shadow(VSInputP IN)
{
	const float4x4 mvl = mul(PF.View, PO.Model);
	const float4x4 mvpl = mul(PF.Projection, mvl);

	VS_Output VSOut;
	VSOut.position = mul(mvpl, float4(IN.position.xyz, 1.0f));
	VSOut.depthPosition = VSOut.position;
	return VSOut;
}

float4 PS_Shadow(VS_Output VSOut) : SV_TARGET
{
	float depthValue = (VSOut.depthPosition.z / VSOut.depthPosition.w);
	return float4(depthValue, 1.0f, 1.0f, 1.0f);
}