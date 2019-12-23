#include "IDB_SHADER_COMMON_TYPES"

struct OBJMaterial
{
	
	float3 Ka; // Ambient Color
	float _padd0;

	float3 Kd; // Diffuse Color
	float _padd1;

	float3 Ks; // Specular Color
	float _padd2;

	float Ns; // Specular Exponent
	float Ni; // Optical Density
	float d; // Dissolve
	int illum; // Illumination

	bool HasAmbientTexture;
	bool HasDiffuseTexture;
	bool HasSpecularTexture;
	bool HasSpecularHightlightTexture;
	
	bool HasAlphaTexture;
	bool HasBumpTexture;
	float _padd3;
	float _padd4;
}; 

struct VertexShaderInput
{
	float3 position : POSITION;
	float3 normal : NORMAL0;
	float2 texCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 positionVS : SV_POSITION;
	float4 positionWS : POSITION;
	float3 normal     : NORMAL;
	float3 binormal   : BINORMAL;
	float3 tangent    : TANGENT;
	float2 texCoord   : TEXCOORD0;
};

cbuffer PerObject : register(b0)
{
	float4x4 Model;
	float4x4 View;
	float4x4 Projection;
}
cbuffer Material : register(b2)
{
	OBJMaterial Mat;
};

Texture2D AmbientTexture             : register(t0);
Texture2D DiffuseTexture             : register(t1);
Texture2D SpecularTexture            : register(t2);
Texture2D SpecularHightlightTexture  : register(t3);
Texture2D AlphaTexture               : register(t4);
Texture2D BumpTexture                : register(t5);

sampler DefaultSampler               : register(s0);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	const float4x4 mvp = mul(Projection, mul(View, Model));

	float3 t;
	float3 c1 = cross(IN.normal, float3(0.0f, 0.0f, 1.0f));
	float3 c2 = cross(IN.normal, float3(0.0f, 1.0f, 0.0f));
	if (length(c1) > length(c2))
	{
		t = c1;
	}
	else
	{
		t = c2;
	}

	t = normalize(t);
	float3 b = normalize(cross(IN.normal, t));

	VertexShaderOutput OUT;
	OUT.positionVS = mul(mvp, float4(IN.position, 1.0f));
	OUT.positionWS = float4(IN.position, 1.0f);
	OUT.normal = IN.normal;
	OUT.binormal = b;
	OUT.tangent = t;
	OUT.texCoord = IN.texCoord;

	return OUT;
}

PixelShaderOutput PS_main(VertexShaderOutput IN) : SV_TARGET
{
	float4 diffuse = float4(Mat.Kd, 1.0f);

	//
	// Ambient color
	//
	float4 ambient = float4(Mat.Ka, 1.0f);
	if (Mat.HasAmbientTexture)
	{
		float4 ambientTex = AmbientTexture.Sample(DefaultSampler, IN.texCoord);
		if (any(ambient.rgb))
		{
			ambient *= ambientTex;
		}
		else
		{
			ambient = ambientTex;
		}
	}


	//
	// Diffuse color
	//
	if (Mat.HasDiffuseTexture)
	{
		float4 diffuseTex = DiffuseTexture.Sample(DefaultSampler, IN.texCoord);
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
	if (Mat.HasAlphaTexture)
	{
		// If the material has an opacity texture, use that to override the diffuse alpha.
		alpha = AlphaTexture.Sample(DefaultSampler, IN.texCoord).r;
	}




    PixelShaderOutput OUT;
	OUT.PositionWS = IN.positionWS;
	OUT.Diffuse = float4((diffuse).rgb, alpha);
	OUT.Specular = float4(1.0, 1.0, 1.0, 1.0);
	OUT.NormalWS = float4(1.0, 1.0, 1.0, 0.0);
	return OUT;
}