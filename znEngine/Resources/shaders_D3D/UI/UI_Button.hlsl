#include "..\\_gamedata\\shaders_D3D\\UI\UI_Common.h"

#define BUTTON_STATE_IDLE 0
#define BUTTON_STATE_HOVER 1
#define BUTTON_STATE_CLICKED 2
#define BUTTON_STATE_DISABLED 3

cbuffer Material : register(b1)
{
    float4 Color;
	uint   State;
};

Texture2D IdleTexture     : register(t0);
Texture2D HoverTexture    : register(t1);
Texture2D ClickedTexture  : register(t2);
Texture2D DisabledTexture : register(t3);
sampler   DiffuseTextureSampler  : register(s0);

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	
	if (State == BUTTON_STATE_IDLE)
	{
		return IdleTexture.Sample(DiffuseTextureSampler, IN.texCoord) * Color;
	}
	else if (State == BUTTON_STATE_HOVER)
	{
		return HoverTexture.Sample(DiffuseTextureSampler, IN.texCoord) * Color;
	}
	else if (State == BUTTON_STATE_CLICKED)
	{
		return ClickedTexture.Sample(DiffuseTextureSampler, IN.texCoord) * Color;
	}
	else if (State == BUTTON_STATE_DISABLED)
	{
		return DisabledTexture.Sample(DiffuseTextureSampler, IN.texCoord) * Color;
	}
	
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}