#include "CommonInclude.hlsl"
#include "Material.hlsl"
#include "Light.hlsl"



#define RENDERER_FORWARD 0
#define RENDERER_DEFFERED 1


#ifndef RENDERER_TYPE
#pragma message( "RENDERER_TYPE undefined. Default to RENDERER_FORWARD.")
#define RENDERER_TYPE RENDERER_FORWARD
#endif



struct VertexShaderOutput
{
	float4 position     : SV_POSITION;  // Clip space position.
	float3 positionVS   : POSITION;    // View space position.
	float2 texCoord     : TEXCOORD0;    // Texture coordinate
	float3 normalVS     : NORMAL;       // View space normal.
	float3 tangentVS    : TANGENT;      // View space tangent.
	float3 binormalVS   : BINORMAL;     // View space binormal.

#if RENDERER_TYPE == RENDERER_FORWARD
	float4 lightViewPosition : TEXCOORD1;
#endif
};


cbuffer Material : register(b2)
{
	MaterialModel Mat;
};



#if RENDERER_TYPE == RENDERER_FORWARD
StructuredBuffer<Light> Lights  : register(t10);
#endif



StructuredBuffer<PerObject> Instances  : register(t11);
Texture2D TextureShadow : register(t12);
StructuredBuffer<float4x4> Bones : register(t13);


VertexShaderOutput VS_main_Bones(VSInputPTNTBBB IN)
{
	float4 newVertex = float4(0.0f, 0.0f, 0.0f, 0.0f);

	uint alLeastOne = 0;
	for (uint i = 0; i < 4; i++)
	{
		if (IN.boneWeight[i] > 0.0f)
		{
			alLeastOne = 1;
			//uint boneIndexes[4];
			//boneIndexes[0] = (IN.boneIndex & 0xFF000000u >> 24) & 0x000000FFu;
			//boneIndexes[1] = (IN.boneIndex & 0x00FF0000u >> 16) & 0x000000FFu;
			//boneIndexes[2] = (IN.boneIndex & 0x0000FF00u >>  8) & 0x000000FFu;
			//boneIndexes[3] = (IN.boneIndex & 0x000000FFu      ) & 0x000000FFu;

			newVertex += mul(Bones[(IN.boneIndex[i])], float4(IN.position, 1.0f) * IN.boneWeight[i]);
		}
	}
	
	if (alLeastOne == 0)
	{
		newVertex = float4(IN.position, 1.0f);
	}

	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	VertexShaderOutput OUT;
	OUT.position = mul(mvp, newVertex);
	OUT.positionVS = mul(mv, newVertex).xyz;
	//OUT.tangentVS = mul((float3x3)mv, IN.tangent);
	//OUT.binormalVS = mul((float3x3)mv, IN.binormal);
	//OUT.normalVS = mul((float3x3)mv, IN.normal);
	OUT.tangentVS = mul(mv, float4(IN.tangent, 0.0f)).xyz;
	OUT.binormalVS = mul(mv, float4(IN.binormal, 0.0f)).xyz;
	OUT.normalVS = mul(mv, float4(IN.normal, 0.0f)).xyz;
	OUT.texCoord = IN.texCoord;

	return OUT;
}


VertexShaderOutput VS_main(VSInputPTNTB IN)
{
	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	VertexShaderOutput OUT;
	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.positionVS = mul(mv, float4(IN.position, 1.0f)).xyz;
	//OUT.tangentVS = mul((float3x3)mv, IN.tangent);
	//OUT.binormalVS = mul((float3x3)mv, IN.binormal);
	//OUT.normalVS = mul((float3x3)mv, IN.normal);
	OUT.tangentVS = mul(mv, float4(IN.tangent, 0.0f)).xyz;
	OUT.binormalVS = mul(mv, float4(IN.binormal, 0.0f)).xyz;
	OUT.normalVS = mul(mv, float4(IN.normal, 0.0f)).xyz;
	OUT.texCoord = IN.texCoord;

	return OUT;
}




VertexShaderOutput VS_PTN(VSInputPTN IN)
{
	float3 tangent = (float3)0;
	float3 binormal = (float3)0;

	{
		float3 c1 = cross(IN.normal, float3(0.0f, 0.0f, 1.0f));
		float3 c2 = cross(IN.normal, float3(0.0f, 1.0f, 0.0f));

		if (length(c1) > length(c2))
		{
			tangent = c1;
		}
		else
		{
			tangent = c2;
		}

		tangent = normalize(tangent);

		binormal = cross(IN.normal, tangent);
		binormal = normalize(binormal);
	}

	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	VertexShaderOutput OUT;
	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.positionVS = mul(mv, float4(IN.position, 1.0f)).xyz;
	OUT.tangentVS = mul(mv, float4(tangent, 0.0f)).xyz;
	OUT.binormalVS = mul(mv, float4(binormal, 0.0f)).xyz;
	OUT.normalVS = mul(mv, float4(IN.normal, 0.0f)).xyz;
	OUT.texCoord = IN.texCoord;

	//const float4x4 mvl = mul(PL.LightView, PO.Model);
	//const float4x4 mvpl = mul(PL.LightProjection, mvl);
	//OUT.lightViewPosition = mul(mvpl, float4(IN.position, 1.0f));

	return OUT;
}

VertexShaderOutput VS_PTN_Instanced(VSInputPTN IN, uint InstanceID : SV_InstanceID)
{
	float3 tangent = (float3)0;
	float3 binormal = (float3)0;

	{
		float3 c1 = cross(IN.normal, float3(0.0f, 0.0f, 1.0f));
		float3 c2 = cross(IN.normal, float3(0.0f, 1.0f, 0.0f));

		if (length(c1) > length(c2))
		{
			tangent = c1;
		}
		else
		{
			tangent = c2;
		}

		tangent = normalize(tangent);

		binormal = cross(IN.normal, tangent);
		binormal = normalize(binormal);
	}

	PerObject po = Instances[InstanceID];

	const float4x4 mv = mul(PF.View, po.Model);
	const float4x4 mvp = mul(PF.Projection, mv);

	VertexShaderOutput OUT;
	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.positionVS = mul(mv, float4(IN.position, 1.0f)).xyz;
	OUT.tangentVS = mul(mv, float4(tangent, 0.0f)).xyz;
	OUT.binormalVS = mul(mv, float4(binormal, 0.0f)).xyz;
	OUT.normalVS = mul(mv, float4(IN.normal, 0.0f)).xyz;
	OUT.texCoord = IN.texCoord;
	return OUT;
}


float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	MaterialModel mat = Mat;

	float4 diffuseAndAlpha = ExtractDuffuseAndAlpha(mat, IN.texCoord);
	if (diffuseAndAlpha.a < 0.05f)
		discard;
		
	float4 ambient = ExtractAmbient(mat, IN.texCoord);
	float4 emissive = ExtractEmissive(mat, IN.texCoord);
	float4 specular = ExtractSpecular(mat, IN.texCoord);
	
	float4 eyePos = { 0.0f, 0.0f, 0.0f, 1.0f };
	float4 vertexPositionVS = float4(IN.positionVS, 1.0f);
	float4 normalVS = ExtractNormal(mat, IN.texCoord, IN.normalVS, IN.tangentVS, IN.binormalVS);
	
	MaterialForLight matForLight;
	matForLight.SpecularFactor = specular.a;
	
	LightingResult lit = DoLighting(Lights, matForLight, eyePos, vertexPositionVS, normalVS);

	float4 ambientLight  = float4(diffuseAndAlpha.rgb * lit.Ambient.rgb, 1.0f);
	float4 diffuseLight  = float4(diffuseAndAlpha.rgb * lit.Diffuse.rgb, 1.0f);
	float4 specularLight = float4(specular.rgb, 1.0f) * lit.Specular;

	float4 colorResultt = float4(ambientLight.rgb + diffuseLight.rgb + specularLight.rgb, 1.0f);
	return colorResultt;
}
