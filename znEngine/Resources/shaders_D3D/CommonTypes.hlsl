
sampler LinearRepeatSampler     : register(s0);
sampler LinearClampSampler      : register(s1);

struct PerFrame
{
	float4x4 View;
	float4x4 Projection;
};

struct PerLight
{
	float4x4 LightView;
	float4x4 LightProjection;
};


struct PerObject
{
	float4x4 Model;
};




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

struct VSInputPTN_Instanced
{
	float3 position : POSITION;
	float3 normal   : NORMAL;
	float2 texCoord : TEXCOORD0;
	uint   instance : SV_InstanceID;
};

struct VSInputPTNTB
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal   : NORMAL;
	float3 tangent  : TANGENT;
	float3 binormal : BINORMAL;
};

//---

struct PixelShaderOutput
{
    float4 Diffuse              : SV_Target0;   // Diffuse Albedo (R8G8B8_UNORM) Unused (A8_UNORM)
    float4 Specular             : SV_Target1;   // Specular Color (R8G8B8_UNROM) Specular Power(A8_UNORM)
	float4 PositionWS           : SV_Target2;   // Position in View space (R8G8B8_UNORM) MaterialType (A8_UNORM)   
    float4 NormalWS             : SV_Target3;   // View space normal (R32G32B32_FLOAT) Unused (A32_FLOAT)
};