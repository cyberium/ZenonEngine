
struct MaterialModel
{
	float3 Ambient;					    // Ambient color property.
	float AmbientFactor;				// Ambient factor property. This factor is used to attenuate the ambient color.
	// 16

	float3 Diffuse;					    // Diffuse color property.
	float DiffuseFactor;				// Diffuse factor property. This factor is used to attenuate the diffuse color.
	// 16
			    
	float3 Specular;					// Specular property.
	float SpecularFactor;				// Specular factor property. This factor is used to  attenuate the specular color.
	// 16

	float3 Emissive;					// Emissive color property.
	float EmissiveFactor;				// Emissive factor property. This factor is used to attenuate the emissive color.
	// 16

	float3 Reflection;				    // Reflection color property. This property is used to implement reflection mapping.
	float ReflectionFactor;				// Reflection factor property. This property is used to attenuate the reflection color.
	// 16

	bool HasTextureAmbient;
	bool HasTextureDiffuse;
	bool HasTextureSpecular;
	bool HasTextureEmissive;
	// 16

	bool HasTextureShininess;
	bool HasTextureNormalMap;           // NormalMap property. This property can be used to specify the distortion of the surface normals and create the illusion of a bumpy surface.
	bool HasTextureBump;                // Bump property. This property is used to distort the surface normal and create the illusion of a bumpy surface.
	bool HasTextureTransparency;
	// 16

	bool HasTextureReflection;
	bool HasTextureDisplacement;
	float Shininess;                    // Shininess property. This property controls the aspect of the shiny spot. It is the specular exponent in the Phong illumination model.
	float BumpFactor;					// Bump factor property. This factor is used to make a surface more or less bumpy.
	
	bool IsEnviorementMappingEnable;
};

Texture2D TextureAmbient                  : register(t0);
Texture2D TextureDiffuse                  : register(t1);
Texture2D TextureSpecular                 : register(t2);
Texture2D TextureEmissive                 : register(t3);

Texture2D TextureShininess                : register(t4);
Texture2D TextureNormalMap                : register(t5);
Texture2D TextureBump                     : register(t6);
Texture2D TextureTransparency             : register(t7);

Texture2D TextureReflection               : register(t8);
Texture2D TextureDisplacement             : register(t9);

//----------------------------------------------------------------------
//-- Bump & Normal mapping
//----------------------------------------------------------------------

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


float2 DoDisplacementMapping(float3x3 TBN, Texture2D tex, sampler s, float2 uv, float3 viewPosVS, float3 fragPosVS)
{
	float height_scale = 0.035f;
	
	float3 viewDirW = normalize(viewPosVS - fragPosVS);
	float3 viewDir = mul(TBN, viewDirW);
	
		
	//const float minLayers = 8.0;
	//const float maxLayers = 32.0;
	//const float numLayers = lerp(maxLayers, minLayers, abs(dot(float3(0.0f, 1.0f, 0.0f), viewDir)));

	const float numLayers = 32;
	
    const float layerDepth = 1.0f / numLayers;
		
    // величина шага смещения текстурных координат на каждом слое расчитывается на основе вектора P
    float2 P = viewDir.xy * height_scale; 
    float2 deltaTexCoords = P / numLayers;
	
	float2 currentTexCoords = uv;
	float currentDepthMapValue = (1.0f - tex.Sample(s, currentTexCoords).r);
	
	int iter = 0;
	float currentLayerDepth = 0.0f;
	while(currentLayerDepth < currentDepthMapValue)
	{
		currentTexCoords -= deltaTexCoords;
		currentDepthMapValue = (1.0f - tex.Sample(s, currentTexCoords).r);
		currentLayerDepth += layerDepth;
		
		if (iter++ > 32)
			break;
	}

	// находим текстурные координаты перед найденной точкой пересечения, т.е. делаем "шаг назад"
	float2 prevTexCoords = currentTexCoords + deltaTexCoords;

	// находим значения глубин до и после нахождения пересечения для использования в линейной интерполяции
	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = (1.0f - tex.Sample(s, prevTexCoords).r) - currentLayerDepth + layerDepth;
	 
	// интерполяция текстурных координат 
	float weight = afterDepth / (afterDepth - beforeDepth);
	float2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
	return finalTexCoords;   
}


//----------------------------------------------------------------------
//-- Work with material
//----------------------------------------------------------------------

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

float2 ExtractDisplacement(MaterialModel mat, float2 TexCoord, float3 normalVS, float3 tangentVS, float3 binormalVS, float3 cameraPosVS, float3 positionVS)
{
	if (mat.HasTextureDisplacement)
	{
		float3 normalWS = mul(PF.InverseView, float4(normalVS.xyz, 0.0f));
		float3 tangentWS = mul(PF.InverseView, float4(tangentVS.xyz, 0.0f));
		float3 binormalWS = mul(PF.InverseView, float4(binormalVS.xyz, 0.0f));
		float3 viewPosWS = mul(PF.InverseView, float4(cameraPosVS.xyz, 1.0f));
		float3 positionWS = mul(PF.InverseView, float4(positionVS.xyz, 1.0f));
	
		// For most scenes using bump mapping, I have to invert the binormal.
		float3x3 TBN = float3x3(normalize( tangentWS),
								normalize( binormalWS),
								normalize( normalWS));

		float2 newTexCoords = DoDisplacementMapping(TBN, TextureDisplacement, LinearRepeatSampler, TexCoord, viewPosWS, positionWS);
		if(newTexCoords.x > 1.0 || newTexCoords.y > 1.0 || newTexCoords.x < 0.0 || newTexCoords.y < 0.0)
			discard; 
		return newTexCoords;
	}
	
	return TexCoord;
}