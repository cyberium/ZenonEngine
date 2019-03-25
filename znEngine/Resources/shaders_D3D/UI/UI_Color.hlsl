#include "IDB_SHADER_UI_COMMON"

cbuffer Material : register(b1)
{
    float4 Color;
};

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return Color;
}