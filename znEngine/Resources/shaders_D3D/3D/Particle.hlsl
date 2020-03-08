#include "IDB_SHADER_COMMON_INCLUDE"



// Uniforms
cbuffer Material : register(b2)
{
	float4 DiffuseColor0;
};

float4 VS_main(VSInputP IN) : SV_POSITION
{
	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	return mul(mvp, float4(IN.position.xyz, 1.0f));
}

// Geom shader
struct PixelShaderInput
{
	float4 position       : SV_POSITION;
	float2 texcoord       : TEXCOORD0;
};


[maxvertexcount(4)]
void GS_Billboard(point VS_OUTPUT input[1], inout TriangleStream<VS_OUTPUT> OutputStream)
{
	// The point passed in is in the horizontal center of the billboard, and at the bottom vertically. Because of this,
	// we will take the trees width and divide it by two when finding the x axis for the quads vertices.
	float halfWidth = 16.0f/*treeBillWidth*/ / 2.0f;

	// The billboard will only be rotated on the y axis, so it's up vector will always be 0,1,0. Because of this, we can
	// find the billboards vertices using the cameras position and the billboards position. We start by getting the billboards
	// plane normal:
	float3 planeNormal = input[0].worldPos - camPos;
	planeNormal.y = 0.0f;
	planeNormal = normalize(planeNormal);

	float3 upVector = float3(0.0f, 1.0f, 0.0f);

	// Now we need to find the billboards right vector, so we can easily find the billboards vertices from the input point
	float3 rightVector = normalize(cross(planeNormal, upVector)); // Cross planes normal with the up vector (+y) to get billboards right vector

	rightVector = rightVector * halfWidth; // change the length of the right vector to be half the width of the billboard

	// Get the billboards "height" vector which will be used to find the top two vertices in the billboard quad
	upVector = float3(0, treeBillHeight, 0);

	// Create the billboards quad
	float3 vert[4];

	// We get the points by using the billboards right vector and the billboards height
	vert[0] = input[0].worldPos - rightVector; // Get bottom left vertex
	vert[1] = input[0].worldPos + rightVector; // Get bottom right vertex
	vert[2] = input[0].worldPos - rightVector + upVector; // Get top left vertex
	vert[3] = input[0].worldPos + rightVector + upVector; // Get top right vertex

	// Get billboards texture coordinates
	float2 texCoord[4];
	texCoord[0] = float2(0, 1);
	texCoord[1] = float2(1, 1);
	texCoord[2] = float2(0, 0);
	texCoord[3] = float2(1, 0);

	// Now we "append" or add the vertices to the outgoing stream list
	VS_OUTPUT outputVert;
	for (int i = 0; i < 4; i++)
	{
		outputVert.Pos = mul(float4(vert[i], 1.0f), WVP);
		outputVert.worldPos = float4(vert[i], 0.0f);
		outputVert.TexCoord = texCoord[i];

		// These will not be used for billboards
		outputVert.normal = float3(0, 0, 0);
		outputVert.tangent = float3(0, 0, 0);

		OutputStream.Append(outputVert);
	}
}

DefferedRenderPSOut PS_main() : SV_TARGET
{
	float4 DiffuseColor = float4(0.0f, 1.0f, 0.0f, 1.0f);
	DefferedRenderPSOut OUT;
	OUT.Diffuse = DiffuseColor;
	OUT.Specular = DiffuseColor;
	OUT.NormalWS = float4(1.0f, 1.0f, 1.0f, 0.0f);
	return OUT;
}