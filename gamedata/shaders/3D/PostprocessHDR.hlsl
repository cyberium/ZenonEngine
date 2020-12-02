#include "3D/PostprocessBase.hlsl"

float4 DoPostrocessEffect(float4 SourceColor, int2 TexCoord, uint SampleIndex)
{
    const float gamma = 1.2f;
	const float exposure = 1.5f;

    // тональная компрессия с экспозицией
    float3 mapped = float3(1.0f, 1.0f, 1.0f) - exp(-SourceColor.rgb * exposure);
    // гамма-коррекция
    mapped = pow(mapped, float3(1.0f, 1.0f, 1.0f) / gamma);

    // тональная компрессия
    //float3 mapped = SourceColor.rgb / (SourceColor.rgb + float3(1.0f, 1.0f, 1.0f));
    // гамма-коррекция
    //mapped = pow(mapped, float3(1.0f, 1.0f, 1.0f) / gamma);

	return float4(mapped, 1.0f);
}
