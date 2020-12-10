#include "CommonInclude.hlsl"
#include "Light.hlsl"
#include "3D/ShadowsUtils.hlsl"

#ifndef MULTISAMPLED
#pragma message( "MULTISAMPLED undefined. Default to 1.")
#define MULTISAMPLED 1
#endif

struct VS_Input
{
	float2 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VS_Output
{
	float4 position : SV_POSITION;
};


cbuffer GPUDefferedMergeShadowMaps : register(b4)
{
	float4x4    LightViewMatrix;
	//--------------------------------------------------------------( 64 bytes )
	float4x4    LightProjectionMatrix;
	//--------------------------------------------------------------( 64 bytes )
};

#if MULTISAMPLED > 1
Texture2DMS<float4, MULTISAMPLED> TextureDepthStencil : register(t4);
#else
Texture2D                         TextureDepthStencil : register(t4);
#endif

Texture2D                         ShadowMapTexture    : register(t5);


VS_Output VS_ScreenQuad(VS_Input IN)
{
	VS_Output VSOut;
	VSOut.position = float4(IN.position, 0.0f, 1.0f);
	return VSOut;
}



[earlydepthstencil]
float4 PS_Deferred_MergeShadows(VS_Output VSOut
#if MULTISAMPLED > 1
, uint SampleIndex : SV_SampleIndex
#endif
) : SV_Target
{
	const int2 texCoord = VSOut.position.xy;

#if MULTISAMPLED > 1
	const float depth = TextureDepthStencil.Load(texCoord, SampleIndex).r;
#else
	const float depth = TextureDepthStencil.Load(int3(texCoord, 0)).r;
#endif
	
	const float4 PView = ScreenToView(float4(texCoord, depth, 1.0f));

	const float4 PModel = mul(PF.InverseView, PView);
	float shadowFactor = IsShadowed(LightProjectionMatrix, LightViewMatrix, ShadowMapTexture, PModel);
	//shadowFactor -= 0.1f;
	//shadowFactor = saturate(shadowFactor);
	
	return float4(shadowFactor, 0.0f, 0.0f, 0.0f);
}