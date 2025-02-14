#include "CommonInclude.hlsl"
#include "Material.hlsl"
#include "Light.hlsl"

cbuffer Material : register(b2)
{
	MaterialModel Mat;
};

StructuredBuffer<SGPULightVS> GPULightsVS : register(t14);
TextureCube TextureSky : register(t15);

float4 PS_main(VSOutput IN) : SV_TARGET
{
	float2 displacedTexCoord = ExtractDisplacement(Mat, LinearClampSampler, IN.texCoord, IN.normalVS.xyz, IN.tangentVS.xyz, IN.binormalVS.xyz, float3(0.0f, 0.0f, 0.0f), IN.positionVS.xyz);
	
	float4 diffuseAndAlpha = ExtractDuffuseAndAlpha(Mat, LinearClampSampler, displacedTexCoord);
	if (diffuseAndAlpha.a < 0.05f)
		discard;

	//if (Mat.IsEnviorementMappingEnable)
	//{
	//	const float3 refl = mul((float3x3)PF.InverseView, reflect(-normalize(float3(0.0f, 0.0f, 0.0f) - IN.positionVS), normalize(IN.normalVS)));
	//	diffuseAndAlpha *= TextureSky.Sample(LinearClampSampler, normalize(refl));
	//}
	
	//float4 ambient = ExtractAmbient(Mat, LinearClampSampler, displacedTexCoord);
	float4 emissive = ExtractEmissive(Mat, LinearClampSampler, displacedTexCoord);
	float4 specular = ExtractSpecular(Mat, LinearClampSampler, displacedTexCoord);
	
	float4 eyePos = { 0.0f, 0.0f, 0.0f, 1.0f };
	float4 positionVS = float4(IN.positionVS, 1.0f);
	float4 normalVS = ExtractNormal(Mat, LinearClampSampler, displacedTexCoord, IN.normalVS, IN.tangentVS, IN.binormalVS);
		
	MaterialForLight matForLight;
	matForLight.SpecularFactor = specular.a;
	
	SLightingResult lit = DoLighting(GPULightsVS, matForLight, eyePos, positionVS, normalVS);

	float3 ambientLight  = diffuseAndAlpha.rgb * lit.Ambient.rgb;
	float3 diffuseLight  = diffuseAndAlpha.rgb * lit.Diffuse.rgb;
	float3 specularLight = specular.rgb        * lit.Specular.rgb;

	float3 resultColor = ambientLight + diffuseLight + specularLight;
	//resultColor += emissive.rgb;
	
	return float4(resultColor, 1.0f);
}
