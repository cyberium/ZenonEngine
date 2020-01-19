#include "IDB_SHADER_COMMON_INCLUDE"

struct VertexShaderOutput
{
	float4 position       : SV_POSITION;
	float3 positionVS     : POSITION;
	float3 normalVS       : NORMAL;
	float3 color          : COLOR;
};

// Uniforms
cbuffer Material : register(b2)
{
	float4 DiffuseColor;
};

VertexShaderOutput VS_main(VSInputPN IN)
{
	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	VertexShaderOutput OUT;
	OUT.position = mul(mvp, float4(IN.position.xyz, 1.0f));
	OUT.positionVS = mul(mv, float4(IN.position.xyz, 1.0f)).xyz;
	OUT.normalVS = normalize(mul(mv, float4(IN.normal.xyz, 0.0f)).xyz);
	return OUT;
}

[maxvertexcount(2)]
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
}

DefferedRenderPSOut PS_main(VertexShaderOutput IN) : SV_TARGET
{
	DefferedRenderPSOut OUT;
	OUT.PositionWS = float4(IN.positionVS, 1.0f);
	OUT.Diffuse = float4(IN.color, 1.0f); // Material.DiffuseColor;
	OUT.Specular = float4(1.0, 1.0, 1.0, 1.0);
	OUT.NormalWS = float4(1.0, 1.0, 1.0, 0.0);
	return OUT;
}