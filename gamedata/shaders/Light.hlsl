//
// Lights functions
//

#define POINT_LIGHT 0
#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2

struct SGPULight
{
	uint   Type;            // Disable or enable the light.
	float  Range;          // The range of the light.
	float  Intensity;      // The intensity of the light.
	float  SpotlightAngle; // The half angle of the spotlight cone.
	//--------------------------------------------------------------( 16 bytes )
	
	float4 AmbientColor;  // Ambient color of the light.
	//--------------------------------------------------------------( 16 bytes )
	
	float4 Color;         // Color of the light. Diffuse and specular colors are not separated.
	//--------------------------------------------------------------( 16 bytes )
};


struct SGPULightVS
{
	SGPULight Struct;
	//--------------------------------------------------------------( 48 bytes )
	float4 LightPositionVS;
	//--------------------------------------------------------------( 16 bytes )
	float4 LightDirectionVS;
	//--------------------------------------------------------------( 16 bytes )
	bool IsEnabled;
	bool IsCastShadow;
	float2 __padding;
	//--------------------------------------------------------------( 16 bytes )
};


struct MaterialForLight
{
	float SpecularFactor;
};


// This lighting result is returned by the lighting functions for each light type.
struct SLightingResult
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
};



float4 DoAmbient(SGPULightVS lightVS)
{
	return lightVS.Struct.AmbientColor;
}

float4 DoDiffuse(SGPULightVS lightVS, float4 L, float4 N)
{
	float NdotL = max(dot(N, L), 0);
	return lightVS.Struct.Color * NdotL;
}

float4 DoSpecular(SGPULightVS lightVS, MaterialForLight mat, float4 V, float4 L, float4 N)
{
	if (mat.SpecularFactor > 1.0f)
	{
		float4 R = normalize(reflect(-L, N));
		float RdotV = max(dot(R, V), 0);

		return lightVS.Struct.Color * pow(RdotV, mat.SpecularFactor);
	}
	
	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}

// Compute the attenuation based on the range of the light.
float DoAttenuation(SGPULightVS lightVS, float d)
{
	return 1.0f - smoothstep(lightVS.Struct.Range * 0.75f, lightVS.Struct.Range, d);
}

float DoSpotCone(SGPULightVS lightVS, float4 L)
{
	// If the cosine angle of the light's direction 
	// vector and the vector from the light source to the point being 
	// shaded is less than minCos, then the spotlight contribution will be 0.
	float minCos = cos(radians(lightVS.Struct.SpotlightAngle));
	
	// If the cosine angle of the light's direction vector
	// and the vector from the light source to the point being shaded
	// is greater than maxCos, then the spotlight contribution will be 1.
	float maxCos = lerp(minCos, 1, 0.5f);
	
	float cosAngle = dot(lightVS.LightDirectionVS, -L);
	
	// Blend between the maxixmum and minimum cosine angles.
	return smoothstep(minCos, maxCos, cosAngle);
}

SLightingResult DoPointLight(SGPULightVS lightVS, MaterialForLight mat, float4 V, float4 P, float4 N)
{
	

	float4 L = lightVS.LightPositionVS - P;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(lightVS, distance);

	SLightingResult result;
	result.Ambient  = DoAmbient(lightVS)                 * attenuation * lightVS.Struct.Intensity;
	result.Diffuse  = DoDiffuse(lightVS, L, N)           * attenuation * lightVS.Struct.Intensity;
	result.Specular = DoSpecular(lightVS, mat, V, L, N)  * attenuation * lightVS.Struct.Intensity;
	return result;
}

SLightingResult DoDirectionalLight(SGPULightVS lightVS, MaterialForLight mat, float4 V, float4 P, float4 N)
{
	float4 L = normalize(-lightVS.LightDirectionVS);

	SLightingResult result;
	result.Ambient  = DoAmbient(lightVS)                 * lightVS.Struct.Intensity;
	result.Diffuse  = DoDiffuse(lightVS, L, N)           * lightVS.Struct.Intensity;
	result.Specular = DoSpecular(lightVS, mat, V, L, N)  * lightVS.Struct.Intensity;
	return result;
}

SLightingResult DoSpotLight(SGPULightVS lightVS, MaterialForLight mat, float4 V, float4 P, float4 N)
{
	float4 L = lightVS.LightPositionVS - P;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(lightVS, distance);
	float spotIntensity = DoSpotCone(lightVS, L);

	SLightingResult result;
	result.Ambient  = DoAmbient(lightVS)                 * attenuation *                 lightVS.Struct.Intensity;
	result.Diffuse  = DoDiffuse(lightVS, L, N)           * attenuation * spotIntensity * lightVS.Struct.Intensity;
	result.Specular = DoSpecular(lightVS, mat, V, L, N)  * attenuation * spotIntensity * lightVS.Struct.Intensity;
	return result;
}

SLightingResult DoLightingSingle(SGPULightVS lightVS, MaterialForLight mat, float4 eyePos, float4 P, float4 N)
{
	if (false == lightVS.IsEnabled)
		return (SLightingResult)0;
	
	if (lightVS.Struct.Type != DIRECTIONAL_LIGHT && length(lightVS.LightPositionVS - P) > lightVS.Struct.Range)
		return (SLightingResult)0; // Skip point and spot lights that are out of range of the point being shaded.

	float4 V = normalize(eyePos - P);
	switch (lightVS.Struct.Type)
	{
		case DIRECTIONAL_LIGHT:
			return DoDirectionalLight(lightVS, mat, V, P, N);
			
		case POINT_LIGHT:
			return DoPointLight(lightVS, mat, V, P, N);
		
		case SPOT_LIGHT:
			return DoSpotLight(lightVS, mat, V, P, N);
	}

	return (SLightingResult)0;
}

SLightingResult DoLighting(StructuredBuffer<SGPULightVS> lightsVS, MaterialForLight mat, float4 eyePos, float4 P, float4 N)
{
	SLightingResult totalLightingResult = (SLightingResult)0;
	
	uint numStructs;
	uint stride;
	lightsVS.GetDimensions(numStructs, stride);
	
	for (uint i = 0; i < numStructs; ++i)
	{
		const SGPULightVS lightVS = lightsVS[i];
		if (false == lightVS.IsEnabled)
			continue;
		
		SLightingResult lightingResult = DoLightingSingle(lightsVS[i], mat, eyePos, P, N);
		totalLightingResult.Ambient += lightingResult.Ambient;
		totalLightingResult.Diffuse += lightingResult.Diffuse;
		totalLightingResult.Specular += lightingResult.Specular;
	}
	return totalLightingResult;
}