#include "..\\_gamedata\\shaders_D3D\\UI\UI_Common.h"

cbuffer Material : register(b1)
{
    float4 Color;
};

Texture2D DiffuseTexture : register(t0);
sampler DiffuseTextureSampler : register(s0);

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return DiffuseTexture.Sample(DiffuseTextureSampler, IN.texCoord) * Color;
}