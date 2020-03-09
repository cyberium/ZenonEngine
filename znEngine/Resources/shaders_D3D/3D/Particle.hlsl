#include "IDB_SHADER_COMMON_INCLUDE"



// Uniforms
cbuffer Material : register(b2)
{
	float4 DiffuseColor0;
};

float4 VS_main(VSInputP IN) : POSITION
{
	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	return /*mul(mvp,*/ float4(IN.position, 1.0f)/*)*/;
	//return mul(mvp, float4(IN.position, 1.0f));
}

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};


[maxvertexcount(4)]
void GS_Billboard(point float4 input[1] : POSITION, inout TriangleStream<PixelShaderInput> OutputStream)
{
	float2 size = float2(16.0f, 16.0f);


	// The point passed in is in the horizontal center of the billboard, and at the bottom vertically. Because of this,
	// we will take the trees width and divide it by two when finding the x axis for the quads vertices.
	float halfWidth = size.x / 2.0f;

	// The billboard will only be rotated on the y axis, so it's up vector will always be 0,1,0. Because of this, we can
	// find the billboards vertices using the cameras position and the billboards position. We start by getting the billboards
	// plane normal:
	float3 planeNormal = input[0].xyz - GetCameraPosition();
	planeNormal.y = 0.0f;
	planeNormal = normalize(planeNormal);

	float3 upVector = float3(0.0f, 1.0f, 0.0f);

	// Now we need to find the billboards right vector, so we can easily find the billboards vertices from the input point
	float3 rightVector = normalize(cross(planeNormal, upVector)); // Cross planes normal with the up vector (+y) to get billboards right vector

	rightVector = rightVector * halfWidth; // change the length of the right vector to be half the width of the billboard

	// Get the billboards "height" vector which will be used to find the top two vertices in the billboard quad
	upVector = float3(0, size.y, 0);
	

	// Create the billboards quad
	float3 vert[4];
	vert[0] = input[0].xyz - rightVector; // Get bottom left vertex
	vert[1] = input[0].xyz + rightVector; // Get bottom right vertex
	vert[2] = input[0].xyz - rightVector + upVector; // Get top left vertex
	vert[3] = input[0].xyz + rightVector + upVector; // Get top right vertex


	// Get billboards texture coordinates
	float2 texCoord[4];
	texCoord[0] = float2(0, 1);
	texCoord[1] = float2(1, 1);
	texCoord[2] = float2(0, 0);
	texCoord[3] = float2(1, 0);

	//const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, PF.View);

	// Now we "append" or add the vertices to the outgoing stream list
	PixelShaderInput outputVert;
	for (int i = 0; i < 4; i++)
	{
		outputVert.position = mul(mvp, float4(vert[i], 1.0f));
		//outputVert.worldPos = float4(vert[i], 0.0f);
		outputVert.texcoord = texCoord[i];

		// These will not be used for billboards
		//outputVert.normal = float3(0, 0, 0);
		//outputVert.tangent = float3(0, 0, 0);

		OutputStream.Append(outputVert);
	}
}

DefferedRenderPSOut PS_main(PixelShaderInput input) : SV_TARGET
{
	float4 DiffuseColor = float4(0.0f, 1.0f, 0.0f, 1.0f);

	DefferedRenderPSOut OUT;
	OUT.Diffuse = DiffuseColor;
	OUT.Specular = DiffuseColor;
	OUT.NormalWS = float4(1.0f, 1.0f, 1.0f, 0.0f);
	return OUT;
}