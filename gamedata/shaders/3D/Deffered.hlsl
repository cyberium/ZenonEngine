#include "CommonInclude.hlsl"

struct VS_Input
{
	float2 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VS_Output
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};


cbuffer LightResult : register(b4)
{
	Light    L;
	float4x4 LightViewMatrix;
	float4x4 LightProjectionMatrix;
	bool     IsShadowEnabled;
};

#ifdef MULTISAMPLED

Texture2DMS<float4, 8> Texture0            : register(t0); // Diffuse
Texture2DMS<float4, 8> Texture1            : register(t1); // Specular
//Texture2DMS<float4, 8> Texture2            : register(t2); // Position
Texture2DMS<float4, 8> Texture3            : register(t3); // Normal
Texture2DMS<float4, 8> TextureDepthStencil : register(t4); // DepthStencil

#else

Texture2D Texture0            : register(t0); // Diffuse
Texture2D Texture1            : register(t1); // Specular
//Texture2D Texture2            : register(t2); // Position
Texture2D Texture3            : register(t3); // Normal
Texture2D TextureDepthStencil : register(t4); // DepthStencil

#endif

Texture2D TextureShadow       : register(t5);

VS_Output VS_ScreenQuad(VS_Input IN)
{
	VS_Output VSOut;
	VSOut.position = float4(IN.position, 0.0f, 1.0f);
	VSOut.texCoord = IN.texCoord;
	return VSOut;
}

[earlydepthstencil]
float4 PS_ScreenQuad(VS_Output VSOut

#ifdef MULTISAMPLED

, uint SampleIndex : SV_SampleIndex

#endif

) : SV_TARGET
{
#ifdef MULTISAMPLED
	return Texture0.Load(VSOut.texCoord, SampleIndex);
#else
	return Texture0.Load(int3(VSOut.texCoord, 0));
#endif
}



float Blur2(Texture2D Texture, sampler Sampler, float2 Coords)
{
	float2 shadowBlurStep = float2(1.0f / 16384.0f, 1.0f / 16384.0f);

	float sum = 0.0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			float2 offset = shadowBlurStep * float2(i, j);
			sum += Texture.Sample(Sampler, Coords + offset).r;
		}
	}
	return sum / 9;
}


bool IsShadowed(float4 PModel)
{
	const float4x4 mvpl = mul(LightProjectionMatrix, LightViewMatrix);
	const float4 lightViewPosition = mul(mvpl, PModel);

	float2 projectTexCoord = (float2)0;
	projectTexCoord.x = (lightViewPosition.x / lightViewPosition.w) * 0.5f + 0.5f; // From (-1; 1) to (0-1)
	projectTexCoord.y = (-lightViewPosition.y / lightViewPosition.w) * 0.5f + 0.5f;
	float lightDepthValue = (lightViewPosition.z / lightViewPosition.w);

	if (saturate(projectTexCoord.x) == projectTexCoord.x && saturate(projectTexCoord.y) == projectTexCoord.y)
	{
		//float storedDepthValue = TextureShadow.Load(int3(projectTexCoord, 0)).r;
		//float storedDepthValue = Blur2(TextureShadow, LinearClampSampler, projectTexCoord);
		float storedDepthValue = TextureShadow.Sample(LinearClampSampler, projectTexCoord).r;
		//float storedDepthValue = TextureShadow.Load(projectTexCoord, SampleIndex).r;
		//float storedDepthValue = CalcShadowFactor(lightViewPosition);

		return lightDepthValue > storedDepthValue;
	}

	return false;
}

[earlydepthstencil]
float4 PS_DeferredLighting(VS_Output VSOut
#ifdef MULTISAMPLED
, uint SampleIndex : SV_SampleIndex
#endif
) : SV_Target
{
	const int2 texCoord = VSOut.position.xy;

#ifdef MULTISAMPLED
	const float4 diffuse        = Texture0.Load(texCoord, SampleIndex);
	const float4 specular       = Texture1.Load(texCoord, SampleIndex);
	//const float4 nativePosition = Texture2.Load(texCoord, SampleIndex);
	const float4 N              = Texture3.Load(texCoord, SampleIndex);
	const float depth           = TextureDepthStencil.Load(texCoord, SampleIndex).r;
#else
	const float4 diffuse        = Texture0.Load(int3(texCoord, 0));
	const float4 specular       = Texture1.Load(int3(texCoord, 0));
	//const float4 nativePosition = Texture2.Load(int3(texCoord, 0));
	const float4 N              = Texture3.Load(int3(texCoord, 0));
	const float depth           = TextureDepthStencil.Load(int3(texCoord, 0)).r;
#endif

	// Everything is in view space.
	const float4 eyePos = { 0.0f, 0.0f, 0.0f, 1.0f }; 


	const float4 PView = ScreenToView(float4(texCoord, depth, 1.0f));
	const float4 PModel = mul(PF.InverseView, PView);

	// View vector
	float4 V = normalize(eyePos - PView);

	// Unpack the specular power from the alpha component of the specular color.
	float specularPower = exp2(specular.a * 10.5f);

	Light light = L;//Lights[LightIndex];

	MaterialModel mat = (MaterialModel)0;
	mat.Diffuse = diffuse;
	mat.Specular = specular;
	mat.SpecularFactor = specularPower;

	LightingResult lit = (LightingResult)0;

	switch (light.Type)
	{
		case DIRECTIONAL_LIGHT:
		{
			lit = DoDirectionalLight(light, mat, V, PView, N);
		}
		break;
		case POINT_LIGHT:
		{
			lit = DoPointLight(light, mat, V, PView, N);
		}
		break;
		case SPOT_LIGHT:
		{
			lit = DoSpotLight(light, mat, V, PView, N);
		}
		break;
	}

	float4 colorResult = /*lit.Ambient +*/ (diffuse * lit.Diffuse) + (specular * lit.Specular);
	
	if (IsShadowed(PModel))
		return colorResult * lit.Ambient;
	
	return colorResult;
}