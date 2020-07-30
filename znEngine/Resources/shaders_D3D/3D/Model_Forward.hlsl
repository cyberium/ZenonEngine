#include "IDB_SHADER_COMMON_INCLUDE"

#define RENDERER_FORWARD 0
#define RENDERER_DEFFERED 1

#ifndef RENDERER_TYPE
#pragma message( "RENDERER_TYPE undefined. Default to RENDERER_FORWARD.")
#define RENDERER_TYPE RENDERER_FORWARD
#endif

struct VertexShaderOutput
{
	float4 position     : SV_POSITION;  // Clip space position.
	float3 positionVS   : POSITION;    // View space position.
	float2 texCoord     : TEXCOORD0;    // Texture coordinate
	float3 normalVS     : NORMAL;       // View space normal.
	float3 tangentVS    : TANGENT;      // View space tangent.
	float3 binormalVS   : BINORMAL;     // View space binormal.

#if RENDERER_TYPE == RENDERER_FORWARD
	float4 lightViewPosition : TEXCOORD1;
#endif
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

#if RENDERER_TYPE == RENDERER_FORWARD
StructuredBuffer<Light> Lights  : register(t10);
#endif

StructuredBuffer<PerObject> Instances  : register(t11);
Texture2D TextureShadow : register(t12);

VertexShaderOutput VS_main(VSInputPTNTB IN)
{
	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	VertexShaderOutput OUT;
	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.positionVS = mul(mv, float4(IN.position, 1.0f)).xyz;

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
	OUT.positionVS = mul(mv, float4(IN.position, 1.0f)).xyz;
	OUT.tangentVS = mul(mv, float4(tangent, 0.0f)).xyz;
	OUT.binormalVS = mul(mv, float4(binormal, 0.0f)).xyz;
	OUT.normalVS = mul(mv, float4(IN.normal, 0.0f)).xyz;
	OUT.texCoord = IN.texCoord;

	//const float4x4 mvl = mul(PL.LightView, PO.Model);
	//const float4x4 mvpl = mul(PL.LightProjection, mvl);
	//OUT.lightViewPosition = mul(mvpl, float4(IN.position, 1.0f));

	return OUT;
}

VertexShaderOutput VS_PTN_Instanced(VSInputPTN IN, uint InstanceID : SV_InstanceID)
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

	PerObject po = Instances[InstanceID];

	const float4x4 mv = mul(PF.View, po.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	VertexShaderOutput OUT;
	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.positionVS = mul(mv, float4(IN.position, 1.0f)).xyz;
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

	float4 P = float4(IN.positionVS, 1);
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

	LightingResult lit = DoLighting(Lights, mat, eyePos, P, N);
	float4 diffuseLight = diffuse * float4(lit.Diffuse.rgb, 1.0f); // Discard the alpha value from the lighting calculations.

	float4 specularLight = 0;
	if (mat.SpecularFactor > 1.0f) // If specular power is too low, don't use it.
	{
		specularLight = float4(mat.Specular, 1.0);
		if (mat.HasTextureSpecular)
		{
			float4 specularTex = TextureSpecular.Sample(LinearRepeatSampler, IN.texCoord);
			if (any(specularLight.rgb))
			{
				specularLight *= specularTex;
			}
			else
			{
				specularLight = specularTex;
			}
		}
		specularLight *= lit.Specular;
	}

/*

	float4 colorResult = float4((ambient + emissive + diffuseLight + specularLight).rgb, 1.0f
	//alpha * (1.0 - mat.TransparencyFactor)
	);

	float bias = 0.00001f;

	float2 projectTexCoord = (float2)0;
	projectTexCoord.x = (IN.lightViewPosition.x / IN.lightViewPosition.w) * 0.5f + 0.5f; // From (-1; 1) to (0-1)
	projectTexCoord.y = (-IN.lightViewPosition.y / IN.lightViewPosition.w) * 0.5f + 0.5f;

	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		float depthValue = Blur(TextureShadow, LinearClampSampler, projectTexCoord);

		float lightDepthValue = (IN.lightViewPosition.z / IN.lightViewPosition.w) ;
		lightDepthValue -= bias;

		if (lightDepthValue < depthValue)
		{
			//colorResult = float4(lightDepthValue, lightDepthValue, lightDepthValue, 1.0f);
		}
		else
		{
			colorResult *= 0.1f;
		}
	}

*/

	float4 colorResult = float4(lit.Ambient.rgb * ambient.rgb * diffuse.rgb + diffuseLight.rgb + specularLight.rgb, 1.0f);

	DefferedRenderPSOut OUT;
	//OUT.PositionWS = IN.position;
	OUT.Diffuse = diffuse.rgba;//colorResult;
	OUT.Specular = specularLight;
	OUT.NormalWS = float4(1.0, 1.0, 1.0, 0.0);
	return OUT;
}


