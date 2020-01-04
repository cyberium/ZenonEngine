#include "IDB_SHADER_COMMON_TYPES"
#include "IDB_SHADER_COMMON_INCLUDE"

struct VertexShaderInput
{
	float3 position : POSITION;
	float3 tangent  : TANGENT;
	float3 binormal : BINORMAL;
	float3 normal   : NORMAL;
	float2 texCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
	float3 positionVS   : TEXCOORD0;    // View space position.
	float2 texCoord     : TEXCOORD1;    // Texture coordinate
	float3 tangentVS    : TANGENT;      // View space tangent.
	float3 binormalVS   : BINORMAL;     // View space binormal.
	float3 normalVS     : NORMAL;       // View space normal.
	float4 position     : SV_POSITION;  // Clip space position.
};

cbuffer PerObject : register(b0)
{
	float4x4 Model;
	float4x4 View;
	float4x4 Projection;
}
cbuffer Material : register(b2)
{
	Material Mat;
};


Texture2D TextureDiffuse                  : register(t0);
Texture2D TextureEmissive                 : register(t1);
Texture2D TextureAmbient                  : register(t2);
Texture2D TextureSpecular                 : register(t3);
Texture2D TextureShininess                : register(t4);
Texture2D TextureNormalMap                : register(t5);
Texture2D TextureBump                     : register(t6);
Texture2D TextureTransparency             : register(t7);
Texture2D TextureReflection               : register(t8);
Texture2D TextureDisplacement             : register(t9);

StructuredBuffer<Light> Lights  : register(t10);

sampler LinearRepeatSampler     : register(s0);
sampler LinearClampSampler      : register(s1);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	const float4x4 mvp = mul(Projection, mul(View, Model));
	const float4x4 mv = mul(View, Model);

	VertexShaderOutput OUT;
	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.positionVS = mul(mv, float4(IN.position, 1.0f)).xyz;
	OUT.tangentVS = mul((float3x3)mv, IN.tangent);
	OUT.binormalVS = mul((float3x3)mv, IN.binormal);
	OUT.normalVS = mul((float3x3)mv, IN.normal);
	OUT.texCoord = IN.texCoord;
	return OUT;
}

PixelShaderOutput PS_main(VertexShaderOutput IN) : SV_TARGET
{
	// Everything is in view space.
	float4 eyePos = { 0, 0, 0, 1 };
	Material mat = Mat;

	float4 diffuse = float4(mat.Diffuse * mat.DiffuseFactor, 1.0f);
	if (mat.HasTextureDiffuse)
	{
		float4 diffuseTex = TextureDiffuse.Sample(LinearRepeatSampler, IN.texCoord);
		if (any(diffuse.rgb))
		{
			diffuse *= diffuseTex;
		}
		else
		{
			diffuse = diffuseTex;
		}
	}

	// By default, use the alpha from the diffuse component.
	float alpha = diffuse.a;
	if (mat.HasTextureTransparency)
	{
		// If the material has an opacity texture, use that to override the diffuse alpha.
		alpha = TextureTransparency.Sample(LinearRepeatSampler, IN.texCoord).r;
	}

	if (alpha < 0.05)
	{
		discard;
	}

	float4 ambient = float4(mat.Ambient, mat.AmbientFactor);
	if (mat.HasTextureAmbient)
	{
		float4 ambientTex = TextureAmbient.Sample(LinearRepeatSampler, IN.texCoord);
		if (any(ambient.rgb))
		{
			ambient *= ambientTex;
		}
		else
		{
			ambient = ambientTex;
		}
	}
	// Combine the global ambient term.
	//ambient *= mat.GlobalAmbient;

	float4 emissive = float4(mat.Emissive * mat.EmissiveFactor, 1.0f);
	if (mat.HasTextureEmissive)
	{
		float4 emissiveTex = TextureEmissive.Sample(LinearRepeatSampler, IN.texCoord);
		if (any(emissive.rgb))
		{
			emissive *= emissiveTex;
		}
		else
		{
			emissive = emissiveTex;
		}
	}

	//if (mat.HasSpecularPowerTexture)
	//{
	//	mat.SpecularPower = SpecularPowerTexture.Sample(LinearRepeatSampler, IN.texCoord).r * mat.SpecularScale;
	//}

	float4 P = float4(IN.positionVS, 1);
	float4 N;

	// Normal mapping
	if (mat.HasTextureNormalMap)
	{
		// For scense with normal mapping, I don't have to invert the binormal.
		float3x3 TBN = float3x3(normalize(IN.tangentVS),
								normalize(IN.binormalVS),
								normalize(IN.normalVS));

		N = DoNormalMapping(TBN, TextureNormalMap, LinearRepeatSampler, IN.texCoord);
	}
	// Bump mapping
	else if (mat.HasTextureBump)
	{
		// For most scenes using bump mapping, I have to invert the binormal.
		float3x3 TBN = float3x3(normalize(IN.tangentVS),
								normalize(-IN.binormalVS),
								normalize(IN.normalVS));

		N = DoBumpMapping(TBN, TextureBump, LinearRepeatSampler, IN.texCoord, mat.BumpFactor);
	}
	// Just use the normal from the model.
	else
	{
		N = normalize(float4(IN.normalVS, 0));
	}

	

	LightingResult lit = DoLighting(Lights, mat, eyePos, P, N);

	//diffuse *= float4(lit.Diffuse.rgb, 1.0f); // Discard the alpha value from the lighting calculations.

	float4 specular = 0;
	if (mat.SpecularFactor > 1.0f) // If specular power is too low, don't use it.
	{
		specular = float4(mat.Specular * Mat.SpecularFactor, 1.0);
		if (mat.HasTextureSpecular)
		{
			float4 specularTex = TextureSpecular.Sample(LinearRepeatSampler, IN.texCoord);
			if (any(specular.rgb))
			{
				specular *= specularTex;
			}
			else
			{
				specular = specularTex;
			}
		}
		specular *= lit.Specular;
	}

    PixelShaderOutput OUT;
	OUT.PositionWS = IN.position;
	OUT.Diffuse = diffuse;
	//OUT.Diffuse = float4((ambient + emissive + diffuse + specular).rgb, alpha * mat.Opacity);
	OUT.Specular = float4(1.0, 1.0, 1.0, 1.0);
	OUT.NormalWS = float4(1.0, 1.0, 1.0, 0.0);
	return OUT;
}