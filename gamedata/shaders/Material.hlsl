
#define DISPLACEMENT_HEIGHT 0.035f
#define DISPLACEMENT_LAYERS_COUNT 32u

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


float2 DoDisplacementMapping(float3x3 TBN, Texture2D tex, sampler s, float2 uv, float3 viewPosition, float3 pixelPosition)
{
	const float3 viewDirection = normalize(viewPosition - pixelPosition);
	const float3 viewDirTBN = mul(TBN, viewDirection);
	
    const float layerDepth = 1.0f / DISPLACEMENT_LAYERS_COUNT;
		
    // величина шага смещения текстурных координат на каждом слое расчитывается на основе вектора P
    float2 P = viewDirTBN.xy * DISPLACEMENT_HEIGHT; 
    float2 deltaTexCoords = P / DISPLACEMENT_LAYERS_COUNT;
	
	float2 currentTexCoords = uv;
	float currentDepthMapValue = (1.0 - tex.Sample(s, currentTexCoords).r);
	float currentLayerDepth = 0.0f;
	
	for (uint i = 0u; i < DISPLACEMENT_LAYERS_COUNT; i++)
	{
		if (currentLayerDepth >= currentDepthMapValue)
			break;
			
		currentTexCoords -= deltaTexCoords;
		currentDepthMapValue = (1.0 - tex.Sample(s, currentTexCoords).r);
		currentLayerDepth += layerDepth;
	}
	
	// находим текстурные координаты перед найденной точкой пересечения, т.е. делаем "шаг назад"
	float2 prevTexCoords = currentTexCoords + deltaTexCoords;

	// находим значения глубин до и после нахождения пересечения для использования в линейной интерполяции
	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = (1.0 - tex.Sample(s, prevTexCoords).r) - currentLayerDepth + layerDepth;
	 
	// интерполяция текстурных координат 
	float weight = afterDepth / (afterDepth - beforeDepth);
	float2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
	return finalTexCoords;   
}


//----------------------------------------------------------------------
//-- Work with material
//----------------------------------------------------------------------

float4 ExtractDuffuseAndAlpha(MaterialModel mat, const sampler Sampler, float2 TexCoord)
{
	float4 diffuse = float4(mat.Diffuse * mat.DiffuseFactor, 1.0f);
	if (mat.HasTextureDiffuse)
	{
		float4 diffuseTex = TextureDiffuse.Sample(Sampler, TexCoord);
		if (any(diffuse.rgb))
			diffuse *= diffuseTex;
		else
			diffuse = diffuseTex;
	}

	// By default, use the alpha from the diffuse component.
	if (mat.HasTextureTransparency)
		diffuse.a = TextureTransparency.Sample(Sampler, TexCoord).r;
	
	return diffuse;
}

float4 ExtractAmbient(MaterialModel mat, const sampler Sampler, float2 TexCoord)
{
	float4 ambient = float4(mat.Ambient, mat.AmbientFactor);
	if (mat.HasTextureAmbient)
	{
		float4 ambientTex = TextureAmbient.Sample(Sampler, TexCoord);
		if (any(ambient.rgb))
			ambient *= ambientTex;
		else
			ambient = ambientTex;
	}
	return ambient;
}

float4 ExtractEmissive(MaterialModel mat, const sampler Sampler, float2 TexCoord)
{
	float4 emissive = float4(mat.Emissive * mat.EmissiveFactor, 1.0f);
	if (mat.HasTextureEmissive)
	{
		float4 emissiveTex = TextureEmissive.Sample(Sampler, TexCoord);
		if (any(emissive.rgb))
			emissive *= emissiveTex;
		else
			emissive = emissiveTex;
	}
	return emissive;
}

float4 ExtractSpecular(MaterialModel mat, const sampler Sampler, float2 TexCoord)
{
	if (mat.SpecularFactor < 1.0f)
		return float4(0.0f, 0.0f, 0.0f, mat.SpecularFactor);

	float4 specular = float4(mat.Specular, 1.0f);
	if (mat.HasTextureSpecular)
	{
		float4 specularTex = TextureSpecular.Sample(Sampler, TexCoord);
		if (any(specular.rgb))
			specular *= specularTex;
		else
			specular = specularTex;
	}
	return float4(specular.rgb, mat.SpecularFactor); 
}

float4 ExtractNormal(MaterialModel mat, const sampler Sampler, float2 TexCoord, float3 normalVS, float3 tangentVS, float3 binormalVS)
{
	// Normal mapping
	if (mat.HasTextureNormalMap)
	{
		float3x3 TBN = float3x3(normalize(  tangentVS),
								normalize( binormalVS),
								normalize(  normalVS));

		return DoNormalMapping(TBN, TextureNormalMap, Sampler, TexCoord);
	}
	
	// Bump mapping
	if (mat.HasTextureBump)
	{
		float3x3 TBN = float3x3(normalize( tangentVS),
								normalize(-binormalVS),
								normalize( normalVS));

		return DoBumpMapping(TBN, TextureBump, Sampler, TexCoord, mat.BumpFactor);
	}

	return normalize(float4(normalVS, 0.0f));
}

float2 ExtractDisplacement(MaterialModel mat, const sampler Sampler, float2 TexCoord, float3 normalVS, float3 tangentVS, float3 binormalVS, float3 viewPositionVS, float3 pixelPositionVS)
{
	if (mat.HasTextureDisplacement)
	{
		float3x3 TBN = float3x3(
			normalize(  tangentVS),
			normalize(- binormalVS),
			normalize(  normalVS)
		);

		float2 newTexCoords = DoDisplacementMapping(TBN, TextureDisplacement, Sampler, TexCoord, viewPositionVS, pixelPositionVS);
		if(newTexCoords.x > 1.0 || newTexCoords.y > 1.0 || newTexCoords.x < 0.0 || newTexCoords.y < 0.0)
			discard; 
		return newTexCoords;
	}
	
	return TexCoord;
}
