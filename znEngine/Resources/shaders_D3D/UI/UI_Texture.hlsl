#include "IDB_SHADER_UI_VERTEX_BASE"

cbuffer Material : register(b1)
{
    float4 Color;
};

Texture2D DiffuseTexture : register(t0);
sampler DiffuseTextureSampler : register(s0);

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
    return float4(DiffuseTexture.Sample(DiffuseTextureSampler, float2(IN.texCoord.x, IN.texCoord.y)).rgb, 1.0f);
}