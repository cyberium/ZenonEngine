#include "CommonInclude.hlsl"

struct VertexShaderOutput
{
	float4 position       : SV_POSITION;
	float3 color          : COLOR;
};

// Uniforms
cbuffer Material : register(b2)
{
	float4 DiffuseColor0;
};

struct M2PerObject
{
	float4x4 Model;
	float4   Color;
};
StructuredBuffer<M2PerObject> Instances : register(t3);



float4 VS_main(VSInputP IN) : SV_POSITION
{
	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	return mul(mvp, float4(IN.position.xyz, 1.0f));
}

float4 VS_main_Inst(VSInputP IN, uint InstanceID : SV_InstanceID) : SV_POSITION
{
	const float4x4 mv = mul(PF.View, Instances[InstanceID].Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	return mul(mvp, float4(IN.position.xyz, 1.0f));
}

/*[maxvertexcount(2)]
void GS_main(point VertexShaderOutput vertices[1], inout LineStream<VertexShaderOutput> lineStream)
{
	VertexShaderOutput v0;
	v0.position = vertices[0].position;
	v0.positionVS = vertices[0].positionVS;
	v0.normalVS = vertices[0].normalVS;
	v0.color = float3(1.0f, 1.0, 1.0);
	lineStream.Append(v0);

	VertexShaderOutput v1;
	v1.position = vertices[0].position + mul(PF.Projection, float4(vertices[0].normalVS * 5.05f, 0.0f));
	v1.positionVS = vertices[0].positionVS;
	v1.normalVS = vertices[0].normalVS;
	v1.color = float3(1.0f, 0.0, 0.0);
	lineStream.Append(v1);
}*/

DefferedRenderPSOut PS_main() : SV_TARGET
{
	DefferedRenderPSOut OUT;
	OUT.Diffuse = DiffuseColor0;
	OUT.Specular = DiffuseColor0;
	OUT.NormalWS = float4(1.0f, 1.0f, 1.0f, 0.0f);
	return OUT;
}