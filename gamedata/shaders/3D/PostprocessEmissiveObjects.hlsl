#include "CommonInclude.hlsl"
#include "Material.hlsl"

cbuffer Material : register(b2)
{
	MaterialModel Mat;
};

float4 PS_main(VSOutput IN) : SV_TARGET
{	
	return ExtractEmissive(Mat, IN.texCoord);
}
