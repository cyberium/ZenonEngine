#include "CommonInclude.hlsl"


//
// Structs
//
struct SPSInput
{
	float4 position : SV_POSITION;
	float4 color    : COLOR0;
};

[maxvertexcount(2)]
void GS_Main(triangle VSOutput VSOutput[3], inout LineStream<SPSInput> OutputStream)
{

	float3 P0 = VSOutput[0].positionVS;
	float3 P1 = VSOutput[1].positionVS;
	float3 P2 = VSOutput[2].positionVS;
	      
	float3 P = (P0 + P1 + P2) / 3.0f;
	
	float3 V0 = P0 - P1;
	float3 V1 = P2 - P1;
	
	float3 N = cross(V1, V0);
	N = normalize(N);
	
	SPSInput v0;
	v0.position = mul(PF.Projection, float4(P, 1.0f));
	v0.color = float4(0.0f, 1.0, 0.0, 1.0f);
	OutputStream.Append(v0);

	SPSInput v1;
	v1.position = mul(PF.Projection, float4(P + N * 0.2f, 1.0f));
	v1.color = float4(0.0f, 1.0, 0.0, 1.0f);
	OutputStream.Append(v1);
	
	OutputStream.RestartStrip();	
}

float4 PS_main(SPSInput PSInput) : SV_TARGET
{
	return PSInput.color;
}