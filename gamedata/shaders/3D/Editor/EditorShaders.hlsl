#include "CommonInclude.hlsl"

struct VertexShaderOutput
{
	float4 position       : SV_POSITION;
	float3 color          : COLOR;
};

#ifdef INSTANCED
struct SSelectorPerObject
{
	float4x4 Model;
	float4   Color;
};
StructuredBuffer<SSelectorPerObject> Instances : register(t3);
#endif

//
// Vertex shader
//

VertexShaderOutput VS_main(VSInputP IN
#ifdef INSTANCED
	, uint InstanceID : SV_InstanceID
#endif
)
{
#ifdef INSTANCED
	const SSelectorPerObject instance = Instances[InstanceID];
	const float4x4 m = instance.Model;
#else
	const float4x4 m = PO.Model;
#endif
	const float4x4 mv = mul(PF.View, m);
	const float4x4 mvp = mul(PF.Projection, mv);

	VertexShaderOutput vsOut;
	vsOut.position = mul(mvp, float4(IN.position.xyz, 1.0f));
#ifdef INSTANCED
	vsOut.color = instance.Color.rgb;
#else
	vsOut.color = float3(1.0f, 1.0f, 1.0f);
#endif
	return vsOut;
}

//
// Pixel shader
//

DefferedRenderPSOut PS_main(VertexShaderOutput vsOut) : SV_TARGET
{
	DefferedRenderPSOut OUT;
	OUT.Diffuse = float4(vsOut.color, 1.0f);
	OUT.Specular = float4(1.0f, 1.0f, 1.0f, 1.0f);
	OUT.NormalWS = float4(0.0f, 0.0f, 0.0f, 0.0f);
	return OUT;
}