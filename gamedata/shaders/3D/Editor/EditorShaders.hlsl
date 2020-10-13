#include "CommonInclude.hlsl"

struct VertexShaderOutput
{
	float4 position       : SV_POSITION;
	float3 color          : COLOR;
};

struct SSelectorPerObject
{
	float4x4 Model;
	float4   Color;
};
StructuredBuffer<SSelectorPerObject> Instances : register(t3);


VertexShaderOutput VS_main(VSInputP IN)
{
	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	VertexShaderOutput vsOut;
	vsOut.position = mul(mvp, float4(IN.position.xyz, 1.0f));
	vsOut.color = float3(0.3f, 1.0f, 0.3f);
	return vsOut;
}

VertexShaderOutput VS_main_Inst(VSInputP IN, uint InstanceID : SV_InstanceID)
{
	const SSelectorPerObject instance = Instances[InstanceID];
	const float4x4 mv = mul(PF.View, instance.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	VertexShaderOutput vsOut;
	vsOut.position = mul(mvp, float4(IN.position.xyz, 1.0f));
	vsOut.color = instance.Color.rgb;
	return vsOut;
}

DefferedRenderPSOut PS_main(VertexShaderOutput vsOut) : SV_TARGET
{
	DefferedRenderPSOut OUT;
	OUT.Diffuse = float4(vsOut.color, 1.0f);
	OUT.Specular = float4(1.0f, 1.0f, 1.0f, 1.0f);
	OUT.NormalWS = float4(0.0f, 0.0f, 0.0f, 0.0f);
	return OUT;
}