#include "IDB_SHADER_COMMON_INCLUDE"

struct VertexShaderOutput
{
	float4 position     : SV_POSITION;  // Clip space position.
	float4 positionVS   : POSITION;    // View space position.
	float2 texCoord     : TEXCOORD0;    // Texture coordinate
	float3 normalVS     : NORMAL;       // View space normal.
	float3 tangentVS    : TANGENT;      // View space tangent.
	float3 binormalVS   : BINORMAL;     // View space binormal.
};

cbuffer Material : register(b2)
{
	MaterialModel Mat;
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


VertexShaderOutput VS_main(VSInputPTNTB IN)
{
	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	VertexShaderOutput OUT;
	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.positionVS = mul(mv, float4(IN.position, 1.0f));

	//OUT.tangentVS = mul((float3x3)mv, IN.tangent);
	//OUT.binormalVS = mul((float3x3)mv, IN.binormal);
	//OUT.normalVS = mul((float3x3)mv, IN.normal);

	OUT.tangentVS = mul(mv, float4(IN.tangent, 0.0f)).xyz;
	OUT.binormalVS = mul(mv, float4(IN.binormal, 0.0f)).xyz;
	OUT.normalVS = mul(mv, float4(IN.normal, 0.0f)).xyz;
	OUT.texCoord = IN.texCoord;

	return OUT;
}

VertexShaderOutput VS_PTN(VSInputPTN IN)
{
	float3 tangent = (float3)0;
	float3 binormal = (float3)0;

	{
		float3 c1 = cross(IN.normal, float3(0.0f, 0.0f, 1.0f));
		float3 c2 = cross(IN.normal, float3(0.0f, 1.0f, 0.0f));

		if (length(c1) > length(c2))
		{
			tangent = c1;
		}
		else
		{
			tangent = c2;
		}

		tangent = normalize(tangent);

		binormal = cross(IN.normal, tangent);
		binormal = normalize(binormal);
	}

	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	VertexShaderOutput OUT;
	OUT.position = mul(mvp, float4(IN.position, 1.0f));

	//OUT.positionVS = mul(mv, float4(IN.position, 1.0f)).xyz;
	OUT.positionVS = mul(PO.Model, float4(IN.position, 1.0f));

	OUT.tangentVS = mul(mv, float4(tangent, 0.0f)).xyz;
	OUT.binormalVS = mul(mv, float4(binormal, 0.0f)).xyz;
	OUT.normalVS = mul(mv, float4(IN.normal, 0.0f)).xyz;
	OUT.texCoord = IN.texCoord;

	return OUT;
}

DefferedRenderPSOut PS_main(VertexShaderOutput IN) : SV_TARGET
{
	// Everything is in view space.
	float4 eyePos = { 0, 0, 0, 1 };
	MaterialModel mat = Mat;

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

	float4 N = float4(0.0f, 0.0f, 0.0f, 1.0f);

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


	float4 specular = (float4)0;
	if (mat.SpecularFactor > 1.0f)
	{
		specular = float4(mat.Specular, 1.0);
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
	}

	// Method of packing specular power from "Deferred Rendering in Killzone 2" presentation from Michiel van der Leeuw, Guerrilla (2007)
	mat.SpecularFactor = log2(mat.SpecularFactor) / 10.5f;


	DefferedRenderPSOut OUT;
	OUT.Diffuse = float4(diffuse.rgb, alpha);
	OUT.Specular = float4(specular.rgb, mat.SpecularFactor);
	//OUT.PositionWS = float4(diffuse.rgb, alpha);
	OUT.NormalWS = float4(N.xyz, 0.0);
	return OUT;
}


