#include "3D/PostprocessBase.hlsl"

float4 DoPostrocessEffect(float4 SourceColor, int2 TexCoord, uint SampleIndex)
{
	float brightness = dot(SourceColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 1.0)
        return float4(normalize(SourceColor));
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}
