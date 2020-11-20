#include "CommonInclude.hlsl"
#include "Material.hlsl"
#include "Light.hlsl"

cbuffer Material : register(b2)
{
	MaterialModel Mat;
};

DefferedRenderPSOut PS_main(VSOutput IN) : SV_TARGET
{	
	float2 displacedTexCoord = ExtractDisplacement(Mat, IN.texCoord, IN.normalVS.xyz, IN.tangentVS.xyz, IN.binormalVS.xyz, float3(0.0f, 0.0f, 0.0f), IN.positionVS.xyz);
	
	//float4 diffuseAndAlpha = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 diffuseAndAlpha = ExtractDuffuseAndAlpha(Mat, displacedTexCoord);
	if (diffuseAndAlpha.a < 0.05f)
		discard;
		
	//float4 ambient = ExtractAmbient(Mat, displacedTexCoord); // Not used now.
	float4 emissive = ExtractEmissive(Mat, displacedTexCoord);
	float4 specular = ExtractSpecular(Mat, displacedTexCoord);
	float packedSpecularFactor = log2(specular.a) / 10.5f;

	float4 normalVS = ExtractNormal(Mat, float2(displacedTexCoord.x, displacedTexCoord.y), IN.normalVS, IN.tangentVS, IN.binormalVS);

	DefferedRenderPSOut OUT;
	OUT.Diffuse = diffuseAndAlpha;
	OUT.Specular = float4(specular.rgb, packedSpecularFactor);
	OUT.NormalVS = float4(normalVS.xyz, 0.0f);
	OUT.PositionVS = float4(IN.positionVS, 1.0f);
	return OUT;
}
