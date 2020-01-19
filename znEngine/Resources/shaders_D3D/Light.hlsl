//
// Lights functions
//

#ifndef NUM_LIGHTS
#pragma message( "NUM_LIGHTS undefined. Default to 8.")
#define NUM_LIGHTS 20
#endif

#define POINT_LIGHT 0
#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2
#define UNKNOWN 3

struct Light
{
	float4 PositionWS;    // Position for point and spot lights (World space).
	//--------------------------------------------------------------( 16 bytes )
	
	float4 DirectionWS;   // Direction for spot and directional lights (World space).
	//--------------------------------------------------------------( 16 bytes )
	
	float4 PositionVS;    // Position for point and spot lights (View space).
	//--------------------------------------------------------------( 16 bytes )
	
	float4 DirectionVS;   // Direction for spot and directional lights (View space).
	//--------------------------------------------------------------( 16 bytes )
	
	float4 Color;         // Color of the light. Diffuse and specular colors are not separated.
	//--------------------------------------------------------------( 16 bytes )
	
	uint Type;            // Disable or enable the light.
	float Range;          // The range of the light.
	float Intensity;      // The intensity of the light.
	float SpotlightAngle; // The half angle of the spotlight cone.
	//--------------------------------------------------------------(16 bytes )

	//--------------------------------------------------------------( 16 * 6 = 112 bytes )
};

// This lighting result is returned by the lighting functions for each light type.
struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};

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
	return normalize(float4(normal, 0));
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

float4 DoDiffuse(Light light, float4 L, float4 N)
{
	float NdotL = max(dot(N, L), 0);
	return light.Color * NdotL;
}

float4 DoSpecular(Light light, Material mat, float4 V, float4 L, float4 N)
{
	float4 R = normalize(reflect(-L, N));
	float RdotV = max(dot(R, V), 0);

	return light.Color * pow(RdotV, mat.SpecularFactor);
}

// Compute the attenuation based on the range of the light.
float DoAttenuation(Light light, float d)
{
	return 1.0f - smoothstep(light.Range * 0.75f, light.Range, d);
}

float DoSpotCone(Light light, float4 L)
{
	// If the cosine angle of the light's direction 
	// vector and the vector from the light source to the point being 
	// shaded is less than minCos, then the spotlight contribution will be 0.
	float minCos = cos(radians(light.SpotlightAngle));
	// If the cosine angle of the light's direction vector
	// and the vector from the light source to the point being shaded
	// is greater than maxCos, then the spotlight contribution will be 1.
	float maxCos = lerp(minCos, 1, 0.5f);
	float cosAngle = dot(light.DirectionVS, -L);
	// Blend between the maxixmum and minimum cosine angles.
	return smoothstep(minCos, maxCos, cosAngle);
}

LightingResult DoPointLight(Light light, Material mat, float4 V, float4 P, float4 N)
{
	LightingResult result;

	float4 L = light.PositionVS - P;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);

	result.Diffuse = DoDiffuse(light, L, N) * attenuation * light.Intensity;
	result.Specular = DoSpecular(light, mat, V, L, N) * attenuation * light.Intensity;

	return result;
}

LightingResult DoDirectionalLight(Light light, Material mat, float4 V, float4 P, float4 N)
{
	LightingResult result;

	float4 L = normalize(-light.DirectionVS);

	result.Diffuse = DoDiffuse(light, L, N) * light.Intensity;
	result.Specular = DoSpecular(light, mat, V, L, N) * light.Intensity;

	return result;
}

LightingResult DoSpotLight(Light light, Material mat, float4 V, float4 P, float4 N)
{
	LightingResult result;

	float4 L = light.PositionVS - P;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);
	float spotIntensity = DoSpotCone(light, L);

	result.Diffuse = DoDiffuse(light, L, N) * attenuation * spotIntensity * light.Intensity;
	result.Specular = DoSpecular(light, mat, V, L, N) * attenuation * spotIntensity * light.Intensity;

	return result;
}

LightingResult DoLighting(StructuredBuffer<Light> lights, Material mat, float4 eyePos, float4 P, float4 N)
{
	float4 V = normalize(eyePos - P);

	LightingResult totalResult = (LightingResult)0;

	for (int i = 0; i < NUM_LIGHTS; ++i)
	{
		LightingResult result = (LightingResult)0;

		if (lights[i].Type == UNKNOWN)
		{
			continue;
		}

		// Skip point and spot lights that are out of range of the point being shaded.
		if (lights[i].Type != DIRECTIONAL_LIGHT && length(lights[i].PositionVS - P) > lights[i].Range)
		{
			continue;
		}

		switch (lights[i].Type)
		{
			case DIRECTIONAL_LIGHT:
			{
				result = DoDirectionalLight(lights[i], mat, V, P, N);
			}
			break;
			case POINT_LIGHT:
			{
				result = DoPointLight(lights[i], mat, V, P, N);
			}
			break;
			case SPOT_LIGHT:
			{
				result = DoSpotLight(lights[i], mat, V, P, N);
			}
			break;
		}
		totalResult.Diffuse += result.Diffuse;
		totalResult.Specular += result.Specular;
	}

	return totalResult;
}