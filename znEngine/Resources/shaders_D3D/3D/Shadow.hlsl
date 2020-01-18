#include "IDB_SHADER_COMMON_TYPES"

struct VS_Output
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};

// Uniforms
cbuffer PerObject : register(b0)
{
	PerObject PO;
}
cbuffer PerFrame : register(b1)
{
	PerFrame PF;
}

VS_Output VS_Shadow(VSInputP IN)
{
	// Projection and View - from light
	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	VS_Output VSOut;
	VSOut.position = mul(mvp, float4(IN.position.xyz, 1.0f));
	VSOut.depthPosition = VSOut.position;
	return VSOut;
}

float4 PS_Shadow(VS_Output VSOutput) : SV_TARGET
{
	float depthValue = (VSOutput.depthPosition.z / VSOutput.depthPosition.w);
	return float4(depthValue, 1.0f, 1.0f, 1.0f);
}