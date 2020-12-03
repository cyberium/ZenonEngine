#include "CommonInclude.hlsl"
#include "Material.hlsl"
#include "Light.hlsl"

cbuffer Material : register(b2)
{
	MaterialModel Mat;
};

StructuredBuffer<LightVS> LightsVS : register(t14);
TextureCube TextureSky : register(t15);

float4 PS_main(VSOutput IN) : SV_TARGET
{
	float2 displacedTexCoord = ExtractDisplacement(Mat, IN.texCoord, IN.normalVS.xyz, IN.tangentVS.xyz, IN.binormalVS.xyz, float3(0.0f, 0.0f, 0.0f), IN.positionVS.xyz);
	
	float4 diffuseAndAlpha = ExtractDuffuseAndAlpha(Mat, displacedTexCoord);
	if (diffuseAndAlpha.a < 0.05f)
		discard;

	if (Mat.IsEnviorementMappingEnable)
	{
		const float3 refl = mul((float3x3)PF.InverseView, reflect(-normalize(float3(0.0f, 0.0f, 0.0f) - IN.positionVS), normalize(IN.normalVS)));
		
		diffuseAndAlpha *= TextureSky.Sample(LinearClampSampler, normalize(refl));
	}
	
	//float4 ambient = ExtractAmbient(Mat, displacedTexCoord);
	float4 emissive = ExtractEmissive(Mat, displacedTexCoord);
	float4 specular = ExtractSpecular(Mat, displacedTexCoord);
	
	float4 eyePos = { 0.0f, 0.0f, 0.0f, 1.0f };
	float4 positionVS = float4(IN.positionVS, 1.0f);
	float4 normalVS = ExtractNormal(Mat, displacedTexCoord, IN.normalVS, IN.tangentVS, IN.binormalVS);
	
	MaterialForLight matForLight;
	matForLight.SpecularFactor = specular.a;
	
	SLightingResult lit = DoLighting(LightsVS, matForLight, eyePos, positionVS, normalVS);

	float3 ambientLight  = diffuseAndAlpha.rgb * lit.Ambient.rgb;
	float3 diffuseLight  = diffuseAndAlpha.rgb * lit.Diffuse.rgb;
	float3 specularLight = specular.rgb        * lit.Specular.rgb;

	return float4(ambientLight + diffuseLight + specularLight + emissive.rgb, 1.0f);
}
