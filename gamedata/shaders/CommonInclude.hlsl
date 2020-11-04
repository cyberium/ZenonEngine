#include "Types.hlsl"


//----------------------------------------------------------------------
//-- Global objects
//----------------------------------------------------------------------

cbuffer PerObject : register(b0)
{
	PerObject PO;
}

cbuffer PerFrame : register(b1)
{
	PerFrame PF;
}

// b2 reserved for material

sampler LinearRepeatSampler : register(s0);
sampler LinearClampSampler : register(s1);




float4 ClipToView(float4 clip)       // Convert clip space coordinates to view space
{
	// View space position.
	float4 view = mul(PF.InverseProjection, clip);

	// Perspecitive projection.
	view = view / view.w;

	return view;
}



float4 ScreenToView(float4 screen)   // Convert screen space coordinates to view space.
{
	// Convert to normalized texture coordinates
	float2 texCoord = screen.xy / PF.ScreenDimensions;

	// Convert to clip space
	float4 clip = float4(float2(texCoord.x, 1.0f - texCoord.y) * 2.0f - 1.0f, screen.z, screen.w);

	return ClipToView(clip);
}



float4 ScreenToViewOtrho(float4 screen)  // Convert screen space coordinates to view space.
{
	// Convert to normalized texture coordinates
	float2 texCoord = screen.xy / PF.ScreenDimensions;

	// Convert to clip space
	float4 clip = float4(float2(texCoord.x, 1.0f - texCoord.y) * 2.0f - 1.0f, screen.z, screen.w);

	return mul(PF.InverseProjection, clip);
}



float3 ComputeTangent(float3 Normal)
{
	float3 c1 = cross(Normal, float3(0.0f, 0.0f, 1.0f));
	float3 c2 = cross(Normal, float3(0.0f, 1.0f, 0.0f));

	float3 tangent = (float3)0;
	if (length(c1) > length(c2))
		tangent = c1;
	else
		tangent = c2;
	return normalize(tangent);
}



float3 ComputeBinormal(float3 Normal, float3 Tangent)
{
	return normalize(cross(Normal, Tangent));
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




float3 GetCameraPosition()
{
	return float3(PF.InverseView[0][3], PF.InverseView[1][3], PF.InverseView[2][3]);
}

float3 GetCameraUp()
{
	return float3(PF.InverseView[0][1], PF.InverseView[1][1], PF.InverseView[2][1]);
}
