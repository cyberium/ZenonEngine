#include "IDB_SHADER_UI_COMMON"

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	const float4x4 mp = mul(PF.Projection, PO.Model);

	VertexShaderOutput OUT;
	OUT.position = mul(mp, float4(IN.position, 0.0f, 1.0f));
	OUT.texCoord = IN.texCoord;
	return OUT;
}
