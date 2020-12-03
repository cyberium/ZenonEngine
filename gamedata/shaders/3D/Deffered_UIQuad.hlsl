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


cbuffer LightResult : register(b4)
{
	LightVS  CurrentLightVS;
	float4x4 LightViewMatrix;
	float4x4 LightProjectionMatrix;
	bool     IsShadowEnabled;
};


#if MULTISAMPLED > 1
Texture2DMS<float4, MULTISAMPLED> Texture0            : register(t0); // Diffuse
Texture2DMS<float4, MULTISAMPLED> Texture1            : register(t1); // Specular
Texture2DMS<float4, MULTISAMPLED> Texture3            : register(t2); // Normal
Texture2DMS<float4, MULTISAMPLED> Texture2            : register(t3); // Position
Texture2DMS<float4, MULTISAMPLED> TextureDepthStencil : register(t4);
#else
Texture2D                         Texture0            : register(t0); // Diffuse
Texture2D                         Texture1            : register(t1); // Specular
Texture2D                         Texture3            : register(t2); // Normal
Texture2D                         Texture2            : register(t3); // Position
Texture2D                         TextureDepthStencil : register(t4);
#endif

Texture2D                         TextureShadow       : register(t5);


VS_Output VS_ScreenQuad(VS_Input IN)
{
	VS_Output VSOut;
	VSOut.position = float4(IN.position, 0.0f, 1.0f);
	return VSOut;
}



[earlydepthstencil]
float4 PS_ScreenQuad(VS_Output VSOut
#if MULTISAMPLED > 1
, uint SampleIndex : SV_SampleIndex
#endif
) : SV_TARGET
{
	const int2 texCoord = VSOut.position.xy;

#if MULTISAMPLED > 1
	return Texture0.Load(texCoord, SampleIndex);
#else
	return Texture0.Load(int3(texCoord, 0));
#endif
}




[earlydepthstencil]
float4 PS_DeferredLighting(VS_Output VSOut
#if MULTISAMPLED > 1
, uint SampleIndex : SV_SampleIndex
#endif
) : SV_Target
{
	const int2 texCoord = VSOut.position.xy;

#if MULTISAMPLED > 1
    const float4 diffuseAndAlpha= Texture0.Load(texCoord, SampleIndex);
    const float4 specular       = Texture1.Load(texCoord, SampleIndex);
	const float4 positionVS     = Texture2.Load(texCoord, SampleIndex);
	const float4 normalVS       = Texture3.Load(texCoord, SampleIndex);
	const float depth           = TextureDepthStencil.Load(texCoord, SampleIndex).r;
#else
	const float4 diffuseAndAlpha= Texture0.Load(int3(texCoord, 0));
	const float4 specular       = Texture1.Load(int3(texCoord, 0));
	const float4 positionVS     = Texture2.Load(int3(texCoord, 0));
	const float4 normalVS       = Texture3.Load(int3(texCoord, 0));
	const float depth           = TextureDepthStencil.Load(int3(texCoord, 0)).r;
#endif

	MaterialForLight matForLight;
	matForLight.SpecularFactor = exp2(specular.a * 10.5f);

	const float4 eyePos = { 0.0f, 0.0f, 0.0f, 1.0f }; 
	const float4 PView = ScreenToView(float4(texCoord, depth, 1.0f));
	float4 normal = normalize(float4(normalVS.xyz, 0.0f));

	SLightingResult lit = DoLightingSingle(CurrentLightVS, matForLight, eyePos, PView, normal);
	
	float3 ambientLight  = diffuseAndAlpha.rgb * lit.Ambient.rgb;
	float3 diffuseLight  = diffuseAndAlpha.rgb * lit.Diffuse.rgb;
	float3 specularLight = specular.rgb        * lit.Specular.rgb;
		
	const float4 PModel = mul(PF.InverseView, PView);
	float shadowFactor = IsShadowed(LightProjectionMatrix, LightViewMatrix, TextureShadow, PModel);
	shadowFactor -= 0.1f;
	shadowFactor = saturate(shadowFactor);
	
	if (shadowFactor > 0.0f)
		return float4(ambientLight + (diffuseLight + specularLight) * (1.0f - shadowFactor), 1.0f);
		
	return float4(ambientLight + diffuseLight + specularLight, 1.0f);
}