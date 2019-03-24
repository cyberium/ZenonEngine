#include "..\\_gamedata\\shaders_D3D\\UI\UI_Common.h"

cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
	float4x4 ModelView;
	float4x4 Model;
}

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.position = mul(ModelViewProjection, float4(IN.position, 0.0f, 1.0f));
	OUT.texCoord = IN.texCoord;
	return OUT;
}
