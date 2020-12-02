#include "CommonInclude.hlsl"

#ifndef MULTISAMPLED
#pragma message( "MULTISAMPLED undefined. Default to 1.")
#define MULTISAMPLED 1
#endif

struct VS_Input
{
	float2 position : POSITION;
	float2 texCoord : TEXCOORD0;
};


#if MULTISAMPLED > 1
Texture2DMS<float4, MULTISAMPLED> Texture0 : register(t0);
#else
Texture2D                         Texture0 : register(t0);
#endif


uint2 GetTextureSize()
{
#if MULTISAMPLED > 1
	uint width;
	uint height;
	uint samples;
	Texture0.GetDimensions(width, height, samples);
	return float2(width, height);
#else
	uint width;
	uint height;
	Texture0.GetDimensions(width, height);
	return float2(width, height);
#endif
}


float4 GetTextureColor(float2 TexCoords, uint SampleIndex, float2 Offset = float2(0.0f, 0.0f))
{
#if MULTISAMPLED > 1
	return Texture0.Load(TexCoords, SampleIndex, Offset);
#else
	
	//return Texture0.Sample(LinearClampSampler, TexCoords)
	return Texture0.Load(int3(TexCoords, 0), Offset);
#endif
}


float4 DoPostrocessEffect(float4 SourceColor, int2 TexCoord, uint SampleIndex);


float4 VS_ScreenQuad(VS_Input IN) : SV_POSITION
{
	return float4(IN.position, 0.0f, 1.0f);
}

[earlydepthstencil]
float4 PS_ScreenQuad(float4 position : SV_POSITION
#if MULTISAMPLED > 1
, uint SampleIndex : SV_SampleIndex
#endif
) : SV_TARGET
{
#if MULTISAMPLED <= 1
	uint SampleIndex = 0;
#endif

	return DoPostrocessEffect(GetTextureColor(position.xy, SampleIndex), position.xy, SampleIndex);
}
