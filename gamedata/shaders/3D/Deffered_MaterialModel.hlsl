#include "CommonInclude.hlsl"
#include "Material.hlsl"
#include "Light.hlsl"

cbuffer Material : register(b2)
{
	MaterialModel Mat;
};

TextureCube TextureSky : register(t15);

DefferedRenderPSOut PS_main(VSOutput IN) : SV_TARGET
{	
	float2 displacedTexCoord = ExtractDisplacement(Mat, LinearClampSampler, IN.texCoord, IN.normalVS.xyz, IN.tangentVS.xyz, IN.binormalVS.xyz, float3(0.0f, 0.0f, 0.0f), IN.positionVS.xyz);
	
	float4 diffuseAndAlpha = ExtractDuffuseAndAlpha(Mat, LinearClampSampler, displacedTexCoord);
	if (diffuseAndAlpha.a < 0.05f)
		discard;
				
	//float4 ambient = ExtractAmbient(Mat, LinearClampSampler, displacedTexCoord); // Not used now.
	float4 emissive = ExtractEmissive(Mat, LinearClampSampler, displacedTexCoord);
	float4 specular = ExtractSpecular(Mat, LinearClampSampler, displacedTexCoord);
	float packedSpecularFactor = log2(specular.a) / 10.5f;

	if (Mat.IsEnviorementMappingEnable)
	{
		const float3 refl = mul((float3x3)PF.InverseView, reflect(-normalize(float3(0.0f, 0.0f, 0.0f) - IN.positionVS), normalize(IN.normalVS)));
		diffuseAndAlpha *= TextureSky.Sample(LinearClampSampler, normalize(refl));
	}

	float4 normalVS = ExtractNormal(Mat, LinearClampSampler, float2(displacedTexCoord.x, displacedTexCoord.y), IN.normalVS, IN.tangentVS, IN.binormalVS);

	DefferedRenderPSOut OUT;
	OUT.Diffuse = diffuseAndAlpha;
	OUT.Specular = float4(specular.rgb, packedSpecularFactor);
	OUT.NormalVS = float4(normalVS.xyz, 0.0f);
	OUT.PositionVS = emissive; //float4(IN.positionVS, 1.0f);
	return OUT;
}
