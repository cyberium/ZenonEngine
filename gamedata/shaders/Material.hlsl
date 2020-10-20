
struct MaterialModel
{
	float3 Emissive;					// Emissive color property.
	float EmissiveFactor;				// Emissive factor property. This factor is used to attenuate the emissive color.
	// 16

	float3 Ambient;					    // Ambient color property.
	float AmbientFactor;				// Ambient factor property. This factor is used to attenuate the ambient color.
	// 16

	float3 Diffuse;					    // Diffuse color property.
	float DiffuseFactor;				// Diffuse factor property. This factor is used to attenuate the diffuse color.
	// 16

	float3 NormalMap;				    // NormalMap property. This property can be used to specify the distortion of the surface normals and create the illusion of a bumpy surface.
	float Shininess;                    // Shininess property. This property controls the aspect of the shiny spot. It is the specular exponent in the Phong illumination model.
	// 16

	float3 Bump;						// Bump property. This property is used to distort the surface normal and create the illusion of a bumpy surface.
	float BumpFactor;					// Bump factor property. This factor is used to make a surface more or less bumpy.
	// 16

	float3 TransparentColor;			// Transparent color property.
	float TransparencyFactor;			// Transparency factor property. This property is used to make a surface more or less opaque (0 = opaque, 1 = transparent).
	// 16

	float3 DisplacementColor;           // Displacement color property.
	float DisplacementFactor;			// Displacement factor property.
	// 16

	float3 VectorDisplacementColor;     // Vector displacement color property.
	float VectorDisplacementFactor;		// Vector displacement factor property.
	// 16

	float3 Specular;					// Specular property.
	float SpecularFactor;				// Specular factor property. This factor is used to  attenuate the specular color.
	// 16

	float3 Reflection;				    // Reflection color property. This property is used to implement reflection mapping.
	float ReflectionFactor;				// Reflection factor property. This property is used to attenuate the reflection color.
	// 16
	// 160

	bool HasTextureDiffuse;
	bool HasTextureEmissive;
	bool HasTextureAmbient;
	bool HasTextureSpecular;
	// 16

	bool HasTextureShininess;
	bool HasTextureNormalMap;
	bool HasTextureBump;
	bool HasTextureTransparency;
	// 16

	bool HasTextureReflection;
	bool HasTextureDisplacement;
	float2 Padding;
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



//
// Helpers
//
float3 ExpandNormal(float3 n)
{
	return n * 2.0f - 1.0f;
}

float4 DoNormalMapping(float3x3 TBN, Texture2D tex, sampler s, float2 uv)
{
	float3 normal = tex.Sample(s, uv).xyz;
	normal = ExpandNormal(normal);

	// Transform normal from tangent space to view space.
	normal = mul(normal, TBN);
	return normalize(float4(normal, 0.0f));
}

float4 DoBumpMapping(float3x3 TBN, Texture2D tex, sampler s, float2 uv, float bumpScale)
{
	// Sample the heightmap at the current texture coordinate.
	float height_00 = tex.Sample(s, uv).r * bumpScale;
	
	// Sample the heightmap in the U texture coordinate direction.
	float height_10 = tex.Sample(s, uv, int2(1, 0)).r * bumpScale;
	
	// Sample the heightmap in the V texture coordinate direction.
	float height_01 = tex.Sample(s, uv, int2(0, 1)).r * bumpScale;

	float3 p_00 = { 0, 0, height_00 };
	float3 p_10 = { 1, 0, height_10 };
	float3 p_01 = { 0, 1, height_01 };

	// normal = tangent x bitangent
	float3 normal = cross(normalize(p_10 - p_00), normalize(p_01 - p_00));

	// Transform normal from tangent space to view space.
	normal = mul(normal, TBN);

	return float4(normal, 0);
}




float4 ExtractDuffuseAndAlpha(MaterialModel mat, float2 TexCoord)
{
	float4 diffuse = float4(mat.Diffuse * mat.DiffuseFactor, 1.0f);
	if (mat.HasTextureDiffuse)
	{
		float4 diffuseTex = TextureDiffuse.Sample(LinearRepeatSampler, TexCoord);
		if (any(diffuse.rgb))
			diffuse *= diffuseTex;
		else
			diffuse = diffuseTex;
	}

	// By default, use the alpha from the diffuse component.
	if (mat.HasTextureTransparency)
		diffuse.a = TextureTransparency.Sample(LinearRepeatSampler, TexCoord).r;
	
	return diffuse;
}

float4 ExtractAmbient(MaterialModel mat, float2 TexCoord)
{
	float4 ambient = float4(mat.Ambient, mat.AmbientFactor);
	if (mat.HasTextureAmbient)
	{
		float4 ambientTex = TextureAmbient.Sample(LinearRepeatSampler, TexCoord);
		if (any(ambient.rgb))
			ambient *= ambientTex;
		else
			ambient = ambientTex;
	}
	return ambient;
}

float4 ExtractEmissive(MaterialModel mat, float2 TexCoord)
{
	float4 emissive = float4(mat.Emissive * mat.EmissiveFactor, 1.0f);
	if (mat.HasTextureEmissive)
	{
		float4 emissiveTex = TextureEmissive.Sample(LinearRepeatSampler, TexCoord);
		if (any(emissive.rgb))
			emissive *= emissiveTex;
		else
			emissive = emissiveTex;
	}
	return emissive;
}

float4 ExtractSpecular(MaterialModel mat, float2 TexCoord)
{
	if (mat.SpecularFactor < 1.0f)
		return float4(0.0f, 0.0f, 0.0f, mat.SpecularFactor);

	float4 specular = float4(mat.Specular, 1.0f);
	if (mat.HasTextureSpecular)
	{
		float4 specularTex = TextureSpecular.Sample(LinearRepeatSampler, TexCoord);
		if (any(specular.rgb))
			specular *= specularTex;
		else
			specular = specularTex;
	}
	return float4(specular.rgb, mat.SpecularFactor); 
}

float4 ExtractNormal(MaterialModel mat, float2 TexCoord, float3 normalVS, float3 tangentVS, float3 binormalVS)
{
	// Normal mapping
	if (mat.HasTextureNormalMap)
	{
		// For scense with normal mapping, I don't have to invert the binormal.
		float3x3 TBN = float3x3(normalize(tangentVS),
								normalize(binormalVS),
								normalize(normalVS));

		return DoNormalMapping(TBN, TextureNormalMap, LinearRepeatSampler, TexCoord);
	}
	
	// Bump mapping
	if (mat.HasTextureBump)
	{
		// For most scenes using bump mapping, I have to invert the binormal.
		float3x3 TBN = float3x3(normalize( tangentVS),
								normalize(-binormalVS),
								normalize( normalVS));

		return DoBumpMapping(TBN, TextureBump, LinearRepeatSampler, TexCoord, mat.BumpFactor);
	}

	return normalize(float4(normalVS, 0.0f));
}