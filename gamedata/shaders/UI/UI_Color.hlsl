#include "UI/UI_VertexBase.hlsl"

cbuffer Material : register(b2)
{
    float4 Color;
};

float4 PS_main(VSOutputUI IN) : SV_TARGET
{
	return Color;
}