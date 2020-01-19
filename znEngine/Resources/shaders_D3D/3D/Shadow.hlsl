#include "IDB_SHADER_COMMON_INCLUDE"

/*struct VS_Output
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};*/

float4 VS_Shadow(VSInputP IN) : SV_Position
{
	const float4x4 mvl = mul(PF.View, PO.Model);
	const float4x4 mvpl = mul(PF.Projection, mvl);

	return mul(mvpl, float4(IN.position.xyz, 1.0f));
}

/*float4 PS_Shadow(VS_Output VSOutput) : SV_TARGET
{
	float depthValue = (VSOutput.depthPosition.z / VSOutput.depthPosition.w);
	return float4(depthValue, 1.0f, 1.0f, 1.0f);
}*/