#include "CommonInclude.hlsl"

struct VS_Input
{
	float2 position : POSITION;
	float2 texCoord : TEXCOORD0;
};


#ifdef MULTISAMPLED

Texture2DMS<float4, 8>            Texture0            : register(t0); // Diffuse
Texture2DMS<float4, 8>            TextureDepthStencil : register(t1);

#else

Texture2D                         Texture0            : register(t0); // Diffuse
Texture2D                         TextureDepthStencil : register(t1);

#endif


float4 VS_ScreenQuad(VS_Input IN) : SV_POSITION
{
	return float4(IN.position, 0.0f, 1.0f);
}



[earlydepthstencil]
float4 PS_ScreenQuad(float4 position : SV_POSITION
#ifdef MULTISAMPLED
, uint SampleIndex : SV_SampleIndex
#endif
) : SV_TARGET
{
	const int2 texCoord = position.xy;

#ifdef MULTISAMPLED
	float4 sourceColor = Texture0.Load(texCoord, SampleIndex) * 2.0f;
#else
	float4 sourceColor = Texture0.Load(int3(texCoord, 0));
#endif

    const float gamma = 1.0f;
	const float exposure = 1.0f;

	/*
    // тональная компрессия с экспозицией
    float3 mapped = float3(1.0f, 1.0f, 1.0f) - exp(-sourceColor.rgb * exposure);
    // гамма-коррекция
    mapped = pow(mapped, float3(1.0f, 1.0f, 1.0f) / gamma);
	*/
	

    // тональная компрессия
    float3 mapped = sourceColor.rgb / (sourceColor.rgb + float3(1.0f, 1.0f, 1.0f));
    // гамма-коррекция
    mapped = pow(mapped, float3(1.0f, 1.0f, 1.0f) / gamma);

	return float4(mapped, 1.0f);
}
