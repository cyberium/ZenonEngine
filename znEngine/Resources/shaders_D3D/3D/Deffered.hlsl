#include "IDB_SHADER_COMMON_TYPES"


struct VS_Input
{
	float2 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

Texture2D Texture0 : register(t0); // Diffuse
Texture2D Texture1 : register(t1); // Specular
Texture2D Texture2 : register(t2); // Position
Texture2D Texture3 : register(t3); // Normal

sampler TextureSampler : register(s0);

PixelInputType VS_ScreenQuad(VS_Input IN)
{
	PixelInputType PIT;
	PIT.position = float4(IN.position, 0.0f, 1.0f);
	PIT.texCoord = IN.texCoord;
	return PIT;
}

float4 PS_ScreenQuad(PixelInputType PIT) : SV_TARGET
{
	return Texture0.Sample(TextureSampler, PIT.texCoord) /** Texture1.Sample(TextureSampler, PIT.texCoord) * Texture2.Sample(TextureSampler, PIT.texCoord) * Texture3.Sample(TextureSampler, PIT.texCoord)*/;
}