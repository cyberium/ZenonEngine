#include "CommonInclude.hlsl"

struct VertexShaderOutput
{
	float4 positionVS : SV_POSITION;
	float4 positionWS : POSITION;
	float2 texCoord   : TEXCOORD0;
};

cbuffer Material : register(b2)
{

};

Texture2D DiffuseTexture : register(t0);

VertexShaderOutput VS_main(VSInputPT IN)
{
	const float4x4 mvp = mul(PF.Projection, mul(PF.View, PO.Model));

	VertexShaderOutput OUT;
	OUT.positionVS = mul(mvp, float4(IN.position, 1.0f));
	OUT.positionWS = float4(IN.position, 1.0f);
	OUT.texCoord = IN.texCoord;

	return OUT;
}

DefferedRenderPSOut PS_main(VertexShaderOutput IN) : SV_TARGET
{
	float4 diffuseColor = DiffuseTexture.Sample(LinearRepeatSampler, IN.texCoord);
	if (diffuseColor.a < 0.05f)
		discard;

	DefferedRenderPSOut OUT;
	OUT.Diffuse = diffuseColor;
	OUT.Specular = float4(1.0, 1.0, 1.0, 1.0);
	OUT.NormalWS = float4(1.0, 1.0, 1.0, 0.0);
	return OUT;
}