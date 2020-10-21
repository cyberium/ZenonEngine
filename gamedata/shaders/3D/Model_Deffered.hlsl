#include "CommonInclude.hlsl"
#include "Material.hlsl"
#include "Light.hlsl"

struct VertexShaderOutput
{
	float4 position     : SV_POSITION;  // Clip space position.
	float3 positionVS   : POSITION;    // View space position.
	float2 texCoord     : TEXCOORD0;    // Texture coordinate
	float3 normalVS     : NORMAL;       // View space normal.
	float3 tangentVS    : TANGENT;      // View space tangent.
	float3 binormalVS   : BINORMAL;     // View space binormal.
};

cbuffer Material : register(b2)
{
	MaterialModel Mat;
};

VertexShaderOutput VS_main(VSInputPTNTB IN)
{
	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	VertexShaderOutput OUT;
	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.positionVS = mul(mv, float4(IN.position, 1.0f)).xyz;
	OUT.texCoord = IN.texCoord;
	OUT.normalVS = mul(mv, float4(IN.normal, 0.0f)).xyz;
	OUT.tangentVS = mul(mv, float4(IN.tangent, 0.0f)).xyz;
	OUT.binormalVS = mul(mv, float4(IN.binormal, 0.0f)).xyz;
	return OUT;
}

VertexShaderOutput VS_PTN(VSInputPTN IN)
{
	const float3 tangent = ComputeTangent(IN.normal);
	const float3 binormal = ComputeBinormal(IN.normal, tangent);

	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	VertexShaderOutput OUT;
	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.positionVS = mul(mv, float4(IN.position, 1.0f)).xyz;
	OUT.texCoord = IN.texCoord;
	OUT.normalVS = mul(mv, float4(IN.normal, 0.0f)).xyz;
	OUT.tangentVS = mul(mv, float4(tangent, 0.0f)).xyz;
	OUT.binormalVS = mul(mv, float4(binormal, 0.0f)).xyz;
	return OUT;
}

DefferedRenderPSOut PS_main(VertexShaderOutput IN) : SV_TARGET
{	
	float3 normalWS = mul(PF.InverseView, float4(IN.normalVS.xyz, 0.0f));
	float3 tangentWS = mul(PF.InverseView, float4(IN.tangentVS.xyz, 0.0f));
	float3 binormalWS = mul(PF.InverseView, float4(IN.binormalVS.xyz, 0.0f));

	float3 posCam = mul(PF.InverseView, float4(0.0f, 0.0f, 0.0f, 1.0f));
	float3 posFrag = mul(PF.InverseView, float4(IN.positionVS.xyz, 1.0f));

	float2 newTexCoords = ExtractDisplacement(Mat, IN.texCoord, normalWS, tangentWS, binormalWS, posCam, posFrag);
	if(newTexCoords.x > 1.0 || newTexCoords.y > 1.0 || newTexCoords.x < 0.0 || newTexCoords.y < 0.0)
		discard; 
	
	IN.texCoord = newTexCoords;
	
	
	float4 diffuseAndAlpha = ExtractDuffuseAndAlpha(Mat, IN.texCoord);
	if (diffuseAndAlpha.a < 0.05f)
		discard;
		
	float4 ambient = ExtractAmbient(Mat, IN.texCoord);
	float4 emissive = ExtractEmissive(Mat, IN.texCoord);
	float4 specular = ExtractSpecular(Mat, IN.texCoord);
	float packedSpecularFactor = log2(specular.a) / 10.5f;

	float4 normalVS = ExtractNormal(Mat, IN.texCoord, IN.normalVS, IN.tangentVS, IN.binormalVS);

	DefferedRenderPSOut OUT;
	OUT.Diffuse = diffuseAndAlpha;
	OUT.Specular = float4(specular.rgb, packedSpecularFactor);
	OUT.NormalVS = float4(normalVS.xyz, 0.0f);
	OUT.PositionVS = float4(IN.positionVS, 1.0f);
	return OUT;
}
