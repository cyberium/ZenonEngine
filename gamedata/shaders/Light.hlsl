//
// Lights functions
//

#ifndef NUM_LIGHTS
#pragma message( "NUM_LIGHTS undefined. Default to 1.")
#define NUM_LIGHTS 1
#endif

#define POINT_LIGHT 0
#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2
#define UNKNOWN 3

struct Light
{
	float4 AmbientColor;  // Ambient color of the light.
	//--------------------------------------------------------------( 16 bytes )
	float4 Color;         // Color of the light. Diffuse and specular colors are not separated.
	//--------------------------------------------------------------( 16 bytes )
	uint Type;            // Disable or enable the light.
	float Range;          // The range of the light.
	float Intensity;      // The intensity of the light.
	float SpotlightAngle; // The half angle of the spotlight cone.
	//--------------------------------------------------------------(16 bytes )
};


struct LightVS
{
	Light  Struct;
	float4 LightPositionVS;
	float4 LightDirectionVS;
};


struct MaterialForLight
{
	float SpecularFactor;
};


// This lighting result is returned by the lighting functions for each light type.
struct LightingResult
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
};



float4 DoAmbient(LightVS light)
{
	return light.Struct.AmbientColor;
}

float4 DoDiffuse(LightVS light, float4 L, float4 N)
{
	float NdotL = max(dot(N, L), 0);
	return light.Struct.Color * NdotL;
}

float4 DoSpecular(LightVS light, MaterialForLight mat, float4 V, float4 L, float4 N)
{
	if (mat.SpecularFactor > 1.0f)
	{
		float4 R = normalize(reflect(-L, N));
		float RdotV = max(dot(R, V), 0);

		return light.Struct.Color * pow(RdotV, mat.SpecularFactor);
	}
	
	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}

// Compute the attenuation based on the range of the light.
float DoAttenuation(LightVS light, float d)
{
	return 1.0f - smoothstep(light.Struct.Range * 0.75f, light.Struct.Range, d);
}

float DoSpotCone(LightVS light, float4 L)
{
	// If the cosine angle of the light's direction 
	// vector and the vector from the light source to the point being 
	// shaded is less than minCos, then the spotlight contribution will be 0.
	float minCos = cos(radians(light.Struct.SpotlightAngle));
	
	// If the cosine angle of the light's direction vector
	// and the vector from the light source to the point being shaded
	// is greater than maxCos, then the spotlight contribution will be 1.
	float maxCos = lerp(minCos, 1, 0.5f);
	
	float cosAngle = dot(light.LightDirectionVS, -L);
	
	// Blend between the maxixmum and minimum cosine angles.
	return smoothstep(minCos, maxCos, cosAngle);
}

LightingResult DoPointLight(LightVS light, MaterialForLight mat, float4 V, float4 P, float4 N)
{
	LightingResult result;

	float4 L = light.LightPositionVS - P;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);

	result.Ambient  = DoAmbient(light)                 * attenuation * light.Struct.Intensity;
	result.Diffuse  = DoDiffuse(light, L, N)           * attenuation * light.Struct.Intensity;
	result.Specular = DoSpecular(light, mat, V, L, N)  * attenuation * light.Struct.Intensity;

	return result;
}

LightingResult DoDirectionalLight(LightVS light, MaterialForLight mat, float4 V, float4 P, float4 N)
{
	LightingResult result;

	float4 L = normalize(-light.LightDirectionVS);

	result.Ambient  = DoAmbient(light)                 * light.Struct.Intensity;
	result.Diffuse  = DoDiffuse(light, L, N)           * light.Struct.Intensity;
	result.Specular = DoSpecular(light, mat, V, L, N)  * light.Struct.Intensity;

	return result;
}

LightingResult DoSpotLight(LightVS light, MaterialForLight mat, float4 V, float4 P, float4 N)
{
	LightingResult result;

	float4 L = light.LightPositionVS - P;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);
	float spotIntensity = DoSpotCone(light, L);

	result.Ambient  = DoAmbient(light)                 * attenuation *                 light.Struct.Intensity;
	result.Diffuse  = DoDiffuse(light, L, N)           * attenuation * spotIntensity * light.Struct.Intensity;
	result.Specular = DoSpecular(light, mat, V, L, N)  * attenuation * spotIntensity * light.Struct.Intensity;

	return result;
}

LightingResult DoLightingSingle(LightVS light, MaterialForLight mat, float4 eyePos, float4 P, float4 N)
{
	LightingResult lightingResult = (LightingResult)0;
	if (light.Struct.Type == UNKNOWN)
		return lightingResult;

	if (light.Struct.Type != DIRECTIONAL_LIGHT && length(light.LightPositionVS - P) > light.Struct.Range)
		return lightingResult; // Skip point and spot lights that are out of range of the point being shaded.


	float4 V = normalize(eyePos - P);
	
	switch (light.Struct.Type)
	{
		case DIRECTIONAL_LIGHT:
			return DoDirectionalLight(light, mat, V, P, N);
			
		case POINT_LIGHT:
			return DoPointLight(light, mat, V, P, N);
		
		case SPOT_LIGHT:
			return DoSpotLight(light, mat, V, P, N);
	}

	return lightingResult;
}

LightingResult DoLighting(StructuredBuffer<LightVS> lights, MaterialForLight mat, float4 eyePos, float4 P, float4 N)
{
	LightingResult totalLightingResult = (LightingResult)0;
	for (int i = 0; i < NUM_LIGHTS; ++i)
	{
		LightingResult lightingResult = DoLightingSingle(lights[i], mat, eyePos, P, N);
		totalLightingResult.Ambient += lightingResult.Ambient;
		totalLightingResult.Diffuse += lightingResult.Diffuse;
		totalLightingResult.Specular += lightingResult.Specular;
	}
	return totalLightingResult;
}