
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
//-- Vertex input types (for 3D)
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
	float3 normal   : NORMAL0;
};

struct VSInputPTNTB
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal   : NORMAL0;
	float3 tangent  : TANGENT;
	float3 binormal : BINORMAL;
};

struct VSOutput
{
	float4 position     : SV_POSITION;  // Clip space position.
	float3 positionVS   : POSITION;     // View space position.
	float2 texCoord     : TEXCOORD;     // Texture coordinate
	float3 normalVS     : NORMAL;       // View space normal.
	float3 tangentVS    : TANGENT;      // View space tangent.
	float3 binormalVS   : BINORMAL;     // View space binormal.
};


//----------------------------------------------------------------------
//-- Vertex input types (for UI)
//----------------------------------------------------------------------

struct VSInputUI
{
	float2 position : POSITION;
    float2 texCoord : TEXCOORD0;
};

struct VSOutputUI
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
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