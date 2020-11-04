#include "UI/UI_VertexBase.hlsl"

cbuffer Material : register(b2)
{
    float4 Color;
};

Texture2D DiffuseTexture : register(t0);

float4 PS_main(VSOutputUI IN) : SV_TARGET
{
    return float4(DiffuseTexture.Sample(DiffuseTextureSampler, float2(IN.texCoord.x, 1.0f - IN.texCoord.y)).rgb, 1.0f);
}