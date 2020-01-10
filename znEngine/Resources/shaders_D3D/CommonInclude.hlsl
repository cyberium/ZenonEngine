#ifndef NUM_LIGHTS
#pragma message( "NUM_LIGHTS undefined. Default to 8.")
#define NUM_LIGHTS 20
#endif


#include "IDB_SHADER_MATERIAL"
#include "IDB_SHADER_LIGHT"


struct Plane
{
	float3 N;   // Plane normal.
	float  d;   // Distance to origin.
};

struct Sphere
{
	float3 c;   // Center point.
	float  r;   // Radius.
};

struct Cone
{
	float3 T;   // Cone tip.
	float  h;   // Height of the cone.
	float3 d;   // Direction of the cone.
	float  r;   // bottom radius of the cone.
};

// Four planes of a view frustum (in view space).
// The planes are:
//  * Left,
//  * Right,
//  * Top,
//  * Bottom.
// The back and/or front planes can be computed from depth values in the 
// light culling compute shader.
struct Frustum
{
	Plane planes[4];   // left, right, top, bottom frustum planes.
};



// Parameters required to convert screen space coordinates to view space params.
cbuffer ScreenToViewParams : register(b3)
{
	float4x4 InverseProjection;
	float2 ScreenDimensions;
}



// Convert clip space coordinates to view space
float4 ClipToView(float4 clip)
{
	// View space position.
	float4 view = mul(InverseProjection, clip);
	// Perspecitive projection.
	view = view / view.w;

	return view;
}

// Convert screen space coordinates to view space.
float4 ScreenToView(float4 screen)
{
	// Convert to normalized texture coordinates
	float2 texCoord = screen.xy / ScreenDimensions;

	// Convert to clip space
	float4 clip = float4(float2(texCoord.x, 1.0f - texCoord.y) * 2.0f - 1.0f, screen.z, screen.w);

	return ClipToView(clip);
}

// Compute a plane from 3 noncollinear points that form a triangle.
// This equation assumes a right-handed (counter-clockwise winding order) 
// coordinate system to determine the direction of the plane normal.
Plane ComputePlane(float3 p0, float3 p1, float3 p2)
{
	Plane plane;

	float3 v0 = p1 - p0;
	float3 v2 = p2 - p0;

	plane.N = normalize(cross(v0, v2));

	// Compute the distance to the origin using p0.
	plane.d = dot(plane.N, p0);

	return plane;
}

// Check to see if a sphere is fully behind (inside the negative halfspace of) a plane.
// Source: Real-time collision detection, Christer Ericson (2005)
bool SphereInsidePlane(Sphere sphere, Plane plane)
{
	return dot(plane.N, sphere.c) - plane.d < -sphere.r;
}

// Check to see if a point is fully behind (inside the negative halfspace of) a plane.
bool PointInsidePlane(float3 p, Plane plane)
{
	return dot(plane.N, p) - plane.d < 0;
}

// Check to see if a cone if fully behind (inside the negative halfspace of) a plane.
// Source: Real-time collision detection, Christer Ericson (2005)
bool ConeInsidePlane(Cone cone, Plane plane)
{
	// Compute the farthest point on the end of the cone to the positive space of the plane.
	float3 m = cross(cross(plane.N, cone.d), cone.d);
	float3 Q = cone.T + cone.d * cone.h - m * cone.r;

	// The cone is in the negative halfspace of the plane if both
	// the tip of the cone and the farthest point on the end of the cone to the 
	// positive halfspace of the plane are both inside the negative halfspace 
	// of the plane.
	return PointInsidePlane(cone.T, plane) && PointInsidePlane(Q, plane);
}

// Check to see of a light is partially contained within the frustum.
bool SphereInsideFrustum(Sphere sphere, Frustum frustum, float zNear, float zFar)
{
	bool result = true;

	// First check depth
	// Note: Here, the view vector points in the -Z axis so the 
	// far depth value will be approaching -infinity.
	if (sphere.c.z - sphere.r > zNear || sphere.c.z + sphere.r < zFar)
	{
		result = false;
	}

	// Then check frustum planes
	for (int i = 0; i < 4 && result; i++)
	{
		if (SphereInsidePlane(sphere, frustum.planes[i]))
		{
			result = false;
		}
	}

	return result;
}

bool ConeInsideFrustum(Cone cone, Frustum frustum, float zNear, float zFar)
{
	bool result = true;

	Plane nearPlane = { float3(0, 0, -1), -zNear };
	Plane farPlane = { float3(0, 0, 1), zFar };

	// First check the near and far clipping planes.
	if (ConeInsidePlane(cone, nearPlane) || ConeInsidePlane(cone, farPlane))
	{
		result = false;
	}

	// Then check frustum planes
	for (int i = 0; i < 4 && result; i++)
	{
		if (ConeInsidePlane(cone, frustum.planes[i]))
		{
			result = false;
		}
	}

	return result;
}

float3 ExpandNormal(float3 n)
{
	return n * 2.0f - 1.0f;
}

// This lighting result is returned by the lighting functions for each light type.
struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};

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

float4 DoSpecular(Light light, Material material, float4 V, float4 L, float4 N)
{
	float4 R = normalize(reflect(-L, N));
	float RdotV = max(dot(R, V), 0);

	return light.Color * pow(RdotV, material.SpecularFactor);
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

		// Skip lights that are not enabled.
		if (!lights[i].Enabled)
		{
			continue;
		}

		// Skip point and spot lights that are out of range of the point being shaded.
		//if (lights[i].Type != DIRECTIONAL_LIGHT && length(lights[i].PositionVS - P) > lights[i].Range)
		//{
		//	continue;
		//}

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
