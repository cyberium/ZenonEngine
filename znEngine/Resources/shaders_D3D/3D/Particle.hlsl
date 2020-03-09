#include "IDB_SHADER_COMMON_INCLUDE"


struct Particle
{
	float3 Position;
	float __padding0;
	//--------------------------------------------------------------( 16 bytes )

	float4 Color;
	//--------------------------------------------------------------( 16 bytes )

	float2 Size;
	float __padding1[2];
	//--------------------------------------------------------------( 16 bytes )
};



//
// Structs
//
struct VertexShaderInput
{
	uint VertexID : SV_VertexID;
};

struct GeometryShaderInput
{
	uint VertexID : POSITION;
};

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float4 color    : COLOR0;
};



//
// Uniforms
//
cbuffer Material                     : register(b2)
{
	float4 DiffuseColor0;
};

Texture2D DiffuseTexture             : register(t0 );
StructuredBuffer<Particle> Particles : register(t10);



GeometryShaderInput VS_main(VertexShaderInput VertexIN)
{
	GeometryShaderInput ghi;
	ghi.VertexID = VertexIN.VertexID;
	return ghi;
}


[maxvertexcount(4)]
void GS_Billboard(point GeometryShaderInput input[1], inout TriangleStream<PixelShaderInput> OutputStream)
{
	Particle p = Particles[input[0].VertexID];

	float3 planeNormal = p.Position.xyz - GetCameraPosition();
	planeNormal = normalize(planeNormal);

	float3 upVector = GetCameraUp();

	float3 rightVector = normalize(cross(planeNormal, upVector));
	rightVector *= (p.Size.x / 2.0f);

	upVector *= p.Size.y;
	

	// Create the billboards quad
	float3 vert[4];
	vert[0] = p.Position.xyz - rightVector; // Get bottom left vertex
	vert[1] = p.Position.xyz + rightVector; // Get bottom right vertex
	vert[2] = p.Position.xyz - rightVector + upVector; // Get top left vertex
	vert[3] = p.Position.xyz + rightVector + upVector; // Get top right vertex


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
		outputVert.texcoord = texCoord[i];
		outputVert.color = p.Color;

		OutputStream.Append(outputVert);
	}
}

DefferedRenderPSOut PS_main(PixelShaderInput input) : SV_TARGET
{
	//float4 DiffuseColor = DiffuseTexture.Sample(LinearClampSampler, input.texcoord);
	//if (DiffuseColor.a < 0.01)
	//	discard;

	float4 DiffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

	DefferedRenderPSOut OUT;
	OUT.Diffuse = DiffuseColor * input.color;
	OUT.Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	OUT.NormalWS = float4(1.0f, 1.0f, 1.0f, 0.0f);
	return OUT;
}