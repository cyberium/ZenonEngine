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
	return Texture0.Load(texCoord, SampleIndex) * 2.0f;
#else
	return Texture0.Load(int3(texCoord, 0));
#endif
}
