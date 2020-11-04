#include "CommonInclude.hlsl"

VSOutputUI VS_main(VSInputUI IN)
{
	const float4x4 mp = mul(PF.Projection, PO.Model);
	const float4 pos = mul(mp, float4(IN.position, 0.0f, 1.0f));

	VSOutputUI OUT;
	OUT.position = pos;
	OUT.texCoord = IN.texCoord;
	return OUT;
}
