#include "CommonInclude.hlsl"
#include "Light.hlsl"

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
Texture2DMS<float4, 8> Texture2            : register(t2); // Position
Texture2DMS<float4, 8> Texture3            : register(t3); // Normal
Texture2DMS<float4, 8> TextureDepthStencil : register(t4); // DepthStencil

#else

Texture2D Texture0            : register(t0); // Diffuse
Texture2D Texture1            : register(t1); // Specular
Texture2D Texture2            : register(t2); // Position
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



float Blur2(Texture2D Texture, sampler Sampler, float2 Coords, int Steps, int textureSize)
{
	const float2 shadowBlurStep = float2(1.0f, 1.0f) / textureSize;
	const int cBlurFrom = Steps;
	const float cBlurFromPow2 = ((cBlurFrom * 2) + 1) * ((cBlurFrom * 2) + 1);

	float sum = 0.0f;
	for (int i = -cBlurFrom; i <= cBlurFrom; i++)
		for (int j = -cBlurFrom; j <= cBlurFrom; j++)
			sum += Texture.Sample(Sampler, Coords + shadowBlurStep * float2(i, j)).r;
	return sum / cBlurFromPow2;
}


float IsShadowed(float4 PModel)
{
	const float4x4 mvpl = mul(LightProjectionMatrix, LightViewMatrix);
	const float4 lightViewPosition = mul(mvpl, PModel);

	const float2 projectTexCoord = float2(
		(lightViewPosition.x / lightViewPosition.w) * 0.5f + 0.5f, 
	   -(lightViewPosition.y / lightViewPosition.w) * 0.5f + 0.5f
	);
	const float lightDepthValue = (lightViewPosition.z / lightViewPosition.w);

	const int textureSize = 4096.0f * 2.0f;
	const float2 shadowBlurStep = float2(1.0f, 1.0f) / textureSize;
	const int cBlurFrom = 1;
	const float cBlurFromPow2 = ((cBlurFrom * 2) + 1) * ((cBlurFrom * 2) + 1);

	float sum = 0.0f;
	for (int i = -cBlurFrom; i <= cBlurFrom; i++)
	{
		for (int j = -cBlurFrom; j <= cBlurFrom; j++)
		{
			const float2 offset = shadowBlurStep * float2(i, j);
			const float2 textureCoord = projectTexCoord + offset;
			
			if (saturate(textureCoord.x) == textureCoord.x && saturate(textureCoord.y) == textureCoord.y)
			{
				float storedDepthValue = Blur2(TextureShadow, LinearClampSampler, textureCoord, 0, textureSize);
				//float storedDepthValue = TextureShadow.Sample(LinearClampSampler, textureCoord).r;
				if (lightDepthValue > storedDepthValue)
					sum += (1.0f / float(cBlurFromPow2));
			}
		}
	}

	return sum;
}








float IsShadowed2(float4 PModel)
{
	const float4x4 mvpl = mul(LightProjectionMatrix, LightViewMatrix);
	const float4 lightViewPosition = mul(mvpl, PModel);

	const float2 projectTexCoord = float2(
		(lightViewPosition.x / lightViewPosition.w) * 0.5f + 0.5f, 
	   -(lightViewPosition.y / lightViewPosition.w) * 0.5f + 0.5f
	);
	const float lightDepthValue = (lightViewPosition.z / lightViewPosition.w);

	const int textureSize = 2048.0f;
	const float2 shadowBlurStep = float2(1.0f, 1.0f) / textureSize;
	const int cBlurFrom = 4;
	const float cBlurFromPow2 = ((cBlurFrom * 2) + 1) * ((cBlurFrom * 2) + 1);

	const float PI = 3.14159265f;
	float sigma = 3.3f;
	float3 incrementalGaussian;
	incrementalGaussian.x = 1.0f / (sqrt(2.0f * PI) * sigma);
	incrementalGaussian.y = exp(-0.5f / (sigma * sigma));
    incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;
	
	float avgValue = 0.0f;
	float coefficientSum = 0.0f;
		
	const float2 ts00C = projectTexCoord;
	if (saturate(ts00C.x) == ts00C.x && saturate(ts00C.y) == ts00C.y)
	{
		float ts00 = TextureShadow.Sample(LinearClampSampler, ts00C).r;
		if (lightDepthValue > ts00)
		{
			avgValue += incrementalGaussian.x;
		}
	}
	coefficientSum += incrementalGaussian.x;
	incrementalGaussian.xy *= incrementalGaussian.yz;
	

    // Go through the remaining 8 vertical samples (4 on each side of the center)
	for (int i = -cBlurFrom; i <= cBlurFrom; i++)
	{
		for (int j = -cBlurFrom; j <= cBlurFrom; j++)
		{
			const float2 ts0C = projectTexCoord + float2(i, j) * shadowBlurStep;
			if (saturate(ts0C.x) == ts0C.x && saturate(ts0C.y) == ts0C.y)
			{
				float ts0 = TextureShadow.Sample(LinearClampSampler, ts0C).r;
				if (lightDepthValue >= ts0)
				{
					avgValue += incrementalGaussian.x;
					
					coefficientSum += incrementalGaussian.x;
					incrementalGaussian.xy *= incrementalGaussian.yz;
				}
				else
				{
					coefficientSum += incrementalGaussian.x;
					//incrementalGaussian.xy *= incrementalGaussian.yz;
				}
			}
			else
			{
				coefficientSum += incrementalGaussian.x;
				//incrementalGaussian.xy *= incrementalGaussian.yz;
			}
		}
	}

	return avgValue / coefficientSum;
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
	const float4 positionVS     = Texture2.Load(texCoord, SampleIndex);
	const float4 normalVS       = Texture3.Load(texCoord, SampleIndex);
	const float depth           = TextureDepthStencil.Load(texCoord, SampleIndex).r;
#else
	const float4 diffuse        = Texture0.Load(int3(texCoord, 0));
	const float4 specular       = Texture1.Load(int3(texCoord, 0));
	const float4 positionVS     = Texture2.Load(int3(texCoord, 0));
	const float4 normalVS       = Texture3.Load(int3(texCoord, 0));
	const float depth           = TextureDepthStencil.Load(int3(texCoord, 0)).r;
#endif

	// Everything is in view space.
	const float4 eyePos = { 0.0f, 0.0f, 0.0f, 1.0f }; 
	
	const float4 PView = ScreenToView(float4(texCoord, depth, 1.0f));
	const float4 PModel = mul(PF.InverseView, PView);

	// View vector
	
	float4 N = normalize(float4(normalVS.xyz, 0.0f));
	float4 V = normalize(eyePos - PView);
	
	// Unpack the specular power from the alpha component of the specular color.
	float specularPower = exp2(specular.a * 10.5f);

	Light light = L;//Lights[LightIndex];

	MaterialForLight matForLight;
	matForLight.SpecularFactor = specularPower;

	LightingResult lit = (LightingResult)0;
	switch (light.Type)
	{
		case DIRECTIONAL_LIGHT:
		{
			lit = DoDirectionalLight(light, matForLight, V, PView, N);
		}
		break;
		case POINT_LIGHT:
		{
			lit = DoPointLight(light, matForLight, V, PView, N);
		}
		break;
		case SPOT_LIGHT:
		{
			lit = DoSpotLight(light, matForLight, V, PView, N);
		}
		break;
	}
		

	float4 colorResult = /*lit.Ambient +*/ (diffuse * lit.Diffuse) + (specular * lit.Specular);
	
	float shadowFactor = IsShadowed(PModel);
	shadowFactor -= 0.1f;
	shadowFactor = saturate(shadowFactor);
	
	if (shadowFactor > 0.0f)
		return (diffuse * lit.Diffuse) * (1.0f - shadowFactor);
		
	return colorResult;
}