#include "..\\_gamedata\\shaders_D3D\\UI\UI_Common.h"

cbuffer Material : register(b1)
{
    float4 Color;
};

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return Color;
}