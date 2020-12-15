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
	, uint4  BoneIndexes : BLENDINDICES0
#endif
#ifdef INSTANCED
	, uint InstanceID : SV_InstanceID
#endif
)
{

#ifdef SKELETON_ANIMATION
	float4 vertexPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 vertexNormal = float3(0.0f, 0.0f, 0.0f);

	uint numStructs;
	uint stride;
	Bones.GetDimensions(numStructs, stride);

	bool atLeastOneBone = false;
	for (uint i = 0; i < 4; i++)
	{
		const float boneWeight = BoneWeights[i];	
		if (boneWeight > 0.0f)
		{
			const uint boneIndex = BoneIndexes[i];
			const float4x4 boneMatrix = Bones[boneIndex];
			atLeastOneBone = true;
			vertexPosition += mul(           boneMatrix, float4(IN.position, 1.0f) * boneWeight);
			vertexNormal   += mul((float3x3) boneMatrix,        IN.normal          * boneWeight);
		}
	}
	
	if (false == atLeastOneBone)
	{
		vertexPosition = float4(IN.position, 1.0f);
		vertexNormal = IN.normal;
	}
#else
	float4 vertexPosition = float4(IN.position, 1.0f);
	float3 vertexNormal = IN.normal;
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
	OUT.position = mul(mvp, vertexPosition);
	OUT.positionVS = mul(mv, vertexPosition).xyz;
	OUT.texCoord = float2(IN.texCoord.x, 1.0f - IN.texCoord.y);
	OUT.normalVS = mul((float3x3)mv, vertexNormal).xyz;
	OUT.tangentVS = mul((float3x3)mv, tangent).xyz;
	OUT.binormalVS = mul((float3x3)mv, binormal).xyz;
		
	return OUT;
}