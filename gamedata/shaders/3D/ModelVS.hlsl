#include "CommonInclude.hlsl"

#ifdef INSTANCED
StructuredBuffer<PerObject> Instances : register(t11);
#endif

#ifdef SKELETON_ANIMATION
StructuredBuffer<float4x4> Bones : register(t12);
#endif


VSOutput VS_PTN(VSInputPTN IN
#ifdef SKELETON_ANIMATION
	, float4 BoneWeights: BLENDWEIGHT0
	, float4 BoneWeights2: BLENDWEIGHT1
	, uint4  BoneIndexes : BLENDINDICES0
	, uint4  BoneIndexes2: BLENDINDICES1
#endif
#ifdef INSTANCED
	, uint InstanceID : SV_InstanceID
#endif
)
{
	float3 vertexPosition = float3(0.0f, 0.0f, 0.0f);
	float3 vertexNormal = float3(0.0f, 0.0f, 0.0f);
	
#ifdef SKELETON_ANIMATION
	bool atLeastOneBone = false;

	for (uint i = 0; i < 4; i++)
	{
		const float boneWeight = BoneWeights[i];	
		if (boneWeight > 0.0f)
		{
			atLeastOneBone = true;
			
			const uint boneIndex = BoneIndexes[i];
			const float4x4 boneMatrix = Bones[boneIndex] * boneWeight;
			const float4x4 boneMatrixIT = transpose(inverse(boneMatrix));
			
			vertexPosition += mul(boneMatrix, float4(IN.position, 1.0f)).xyz;			
			vertexNormal += mul((float3x3)boneMatrixIT, IN.normal);
		}
	}
	
	for (uint i = 0; i < 4; i++)
	{
		const float boneWeight = BoneWeights2[i];	
		if (boneWeight > 0.0f)
		{
			atLeastOneBone = true;
		
			const uint boneIndex = BoneIndexes2[i];
			const float4x4 boneMatrix = Bones[boneIndex] * boneWeight;
			const float4x4 boneMatrixIT = transpose(inverse(boneMatrix));
			
			vertexPosition += mul(boneMatrix, float4(IN.position, 1.0f)).xyz;			
			vertexNormal += mul((float3x3)boneMatrixIT, IN.normal);
		}
	}
	
	if (false == atLeastOneBone)
	{
		vertexPosition = IN.position;
		vertexNormal = IN.normal;
	}
	
#else
	vertexPosition = IN.position;
	vertexNormal = IN.normal;
#endif


#ifdef INSTANCED
	const PerObject instance = Instances[InstanceID];
	const float4x4 m = instance.Model;
#else
	const float4x4 m = PO.Model;
#endif

	const float4x4 mv = mul(PF.View, PO.Model);
	const float4x4 mvp = mul(PF.Projection, mv);
	
	const float3 tangent = ComputeTangent(vertexNormal);
	const float3 binormal = ComputeBinormal(vertexNormal, tangent);

	VSOutput OUT;
	OUT.position = mul(mvp, float4(vertexPosition, 1.0f));
	OUT.positionVS = mul(mv, float4(vertexPosition, 1.0f)).xyz;
	OUT.texCoord = float2(IN.texCoord.x, 1.0f - IN.texCoord.y);
	OUT.normalVS = mul((float3x3)mv, vertexNormal).xyz;
	OUT.tangentVS = mul((float3x3)mv, tangent).xyz;
	OUT.binormalVS = mul((float3x3)mv, binormal).xyz;
		
	return OUT;
}