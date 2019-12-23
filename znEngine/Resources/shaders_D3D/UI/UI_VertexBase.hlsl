#include "IDB_SHADER_UI_COMMON"

cbuffer PerObject : register(b0)
{
	float4x4 Model;
	float4x4 Projection;
}

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	const float4x4 mp = mul(Projection, Model);

	VertexShaderOutput OUT;
	OUT.position = mul(mp, float4(IN.position, 0.0f, 1.0f));
	OUT.texCoord = IN.texCoord;
	return OUT;
}
