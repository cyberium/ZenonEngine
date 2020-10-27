
//----------------------------------------------------------------------
//-- Primitives & utils
//----------------------------------------------------------------------

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


struct Frustum
{
	// Four planes of a view frustum (in view space).
	// The planes are: * Left, * Right, * Top, * Bottom.
	// The back and/or front planes can be computed from depth values in the light culling compute shader.
	Plane planes[4];
};



//----------------------------------------------------------------------
//-- Bindings resources for shaders
//----------------------------------------------------------------------

struct PerObject
{
	float4x4 Model;
};

struct PerFrame
{
	float4x4 View;
	float4x4 Projection;
	float4x4 InverseView;
	float4x4 InverseProjection;
	float4x4 InverseViewProjection;
	float2   ScreenDimensions;
};



//----------------------------------------------------------------------
//-- Vertex input types
//----------------------------------------------------------------------

struct VSInputP
{
	float3 position : POSITION;
};

struct VSInputPT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VSInputPN
{
	float3 position : POSITION;
	float3 normal   : NORMAL;
};

struct VSInputPTN
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal   : NORMAL;
};

struct VSInputPTNBB
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal   : NORMAL;
	float4 boneWeight: BLENDWEIGHT0;
	uint4  boneIndex : BLENDINDICES0;
};

struct VSInputPTNTB
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal   : NORMAL;
	float3 tangent  : TANGENT;
	float3 binormal : BINORMAL;
};

struct VSInputPTNTBBB
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal   : NORMAL;
	float3 tangent  : TANGENT;
	float3 binormal : BINORMAL;
	float4 boneWeight: BLENDWEIGHT0;
	uint4  boneIndex : BLENDINDICES0;
};



//----------------------------------------------------------------------
//-- Deffered render types
//----------------------------------------------------------------------

struct DefferedRenderPSOut
{
    float4 Diffuse              : SV_Target0;   // Diffuse Albedo         (R8G8B8_UNORM)     Unused          (A8_UNORM)
    float4 Specular             : SV_Target1;   // Specular Color         (R8G8B8_UNROM)     Specular Power  (A8_UNORM)
	float4 NormalVS             : SV_Target2;   // View space normal      (R32G32B32_FLOAT)  Unused          (A32_FLOAT)
	float4 PositionVS           : SV_Target3;   // Position in View space (R32G32B32_FLOAT)  MaterialType    (A32_FLOAT)
};