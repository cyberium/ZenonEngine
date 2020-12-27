#include "CommonInclude.hlsl"
#include "Material.hlsl"
#include "Light.hlsl"




struct STerrainParams
{
	float3 Scaling;
	float __padding0;
	//--------------------------------------------------------------( 16 bytes )
	
	uint   ImageWidth;
	uint   ImageHeight;
	float __padding1[2];
	//--------------------------------------------------------------( 16 bytes )

};



//
// Structs
//
struct SVSOutput
{
	uint VertexID : POSITION;
};

cbuffer Material : register(b2)
{
	MaterialModel Mat;
};

cbuffer TerrainParams : register(b3)
{
	STerrainParams TerrainParams;
};

Texture2D TerrainNormalMap                 : register(t11);
Texture2D TerrainChannelsMap               : register(t12);
StructuredBuffer<float> Heights            : register(t13);
StructuredBuffer<SGPULightVS> GPULightsVS  : register(t14);
TextureCube TextureSky                     : register(t15);




float GetTerrainHeight(uint X, uint Y)
{
	if (X >= TerrainParams.ImageWidth) X = TerrainParams.ImageWidth - 1u;
	if (Y >= TerrainParams.ImageHeight) Y = TerrainParams.ImageHeight - 1u;
	return Heights[Y * TerrainParams.ImageWidth + X] * 1.00001;;
}

VSOutput GetTerrainVertex(uint X, uint Z, float4x4 mv, float4x4 mvp)
{
	float y = GetTerrainHeight(X, Z);

	const float3 position = float3(X, y, Z) * TerrainParams.Scaling;
	const float3 normal = float3(0.0f, 1.0f, 0.0f); // TODO normalize(TerrainNormalMap.Load(int3(X, Z, 0)).rgb);
	const float3 tangent = ComputeTangent(normal);
	const float3 binormal = ComputeBinormal(normal, tangent);

	VSOutput OUT;
	OUT.position = mul(mvp, float4(position, 1.0f));
	OUT.positionVS = mul(mv, float4(position, 1.0f)).xyz;
	OUT.texCoord = float2(X, Z) / float2(TerrainParams.ImageWidth, TerrainParams.ImageHeight) * 16.0f;
	OUT.normalVS = mul((float3x3)mv, normal).xyz;
	OUT.tangentVS = mul((float3x3)mv, tangent).xyz;
	OUT.binormalVS = mul((float3x3)mv, binormal).xyz;
	return OUT;
}



SVSOutput VS_main(uint VertexID : SV_VertexID)
{
	SVSOutput vsOutput;
	vsOutput.VertexID = VertexID;
	return vsOutput;
}


[maxvertexcount(4)]
void GS_Terrain(point SVSOutput VSOutputt[1], inout TriangleStream<VSOutput> OutputStream)
{
	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	uint vertID = VSOutputt[0].VertexID;

	{	
		uint x = vertID % TerrainParams.ImageWidth;
		uint z = vertID / TerrainParams.ImageHeight;
		OutputStream.Append(GetTerrainVertex(x, z, mv, mvp));
	}

	{	
		uint x = vertID % TerrainParams.ImageWidth;
		uint z = vertID / TerrainParams.ImageHeight + 1u;
		OutputStream.Append(GetTerrainVertex(x, z, mv, mvp));
	}
	
		{	
		uint x = vertID % TerrainParams.ImageWidth + 1u;
		uint z = vertID / TerrainParams.ImageHeight;
		OutputStream.Append(GetTerrainVertex(x, z, mv, mvp));
	}
	
	{	
		uint x = vertID % TerrainParams.ImageWidth + 1u;
		uint z = vertID / TerrainParams.ImageHeight + 1u;
		OutputStream.Append(GetTerrainVertex(x, z, mv, mvp));
	}
}

float4 PS_main(VSOutput IN) : SV_TARGET
{
	float2 displacedTexCoord = ExtractDisplacement(Mat, LinearRepeatSampler, IN.texCoord, IN.normalVS.xyz, IN.tangentVS.xyz, IN.binormalVS.xyz, float3(0.0f, 0.0f, 0.0f), IN.positionVS.xyz);
	
	float4 diffuseAndAlpha = ExtractDuffuseAndAlpha(Mat, LinearRepeatSampler, displacedTexCoord);
	if (diffuseAndAlpha.a < 0.05f)
		discard;

	//if (Mat.IsEnviorementMappingEnable)
	//{
	//	const float3 refl = mul((float3x3)PF.InverseView, reflect(-normalize(float3(0.0f, 0.0f, 0.0f) - IN.positionVS), normalize(IN.normalVS)));
	//	diffuseAndAlpha *= TextureSky.Sample(LinearRepeatSampler, normalize(refl));
	//}
	
	//float4 ambient = ExtractAmbient(Mat, LinearRepeatSampler, displacedTexCoord);
	float4 emissive = ExtractEmissive(Mat, LinearRepeatSampler, displacedTexCoord);
	float4 specular = ExtractSpecular(Mat, LinearRepeatSampler, displacedTexCoord);
	
	float4 eyePos = { 0.0f, 0.0f, 0.0f, 1.0f };
	float4 positionVS = float4(IN.positionVS, 1.0f);
	float4 normalVS = ExtractNormal(Mat, LinearRepeatSampler, displacedTexCoord, IN.normalVS, IN.tangentVS, IN.binormalVS);
		
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
