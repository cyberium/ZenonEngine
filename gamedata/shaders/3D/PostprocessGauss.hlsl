#include "3D/PostprocessBase.hlsl"

cbuffer Parameters : register(b2)
{
	bool IsHorizontal;
};

static const float weight[5] = { 0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f };

float4 DoPostrocessEffect(float4 SourceColor, int2 TexCoord, uint SampleIndex)
{             
    float3 result = SourceColor.rgb * weight[0]; 
	
    if (IsHorizontal)
    {
        for (int i = 1; i < 5; ++i)
        {
            result += GetTextureColor(TexCoord + int2(i, 0), SampleIndex).rgb * weight[i];
            result += GetTextureColor(TexCoord - int2(i, 0), SampleIndex).rgb * weight[i];
        }
    }
    else
    {
        for (int i = 1; i < 5; ++i)
        {
            result += GetTextureColor(TexCoord + int2(0, i), SampleIndex).rgb * weight[i];
            result += GetTextureColor(TexCoord - int2(0, i), SampleIndex).rgb * weight[i];
        }
    }
	
    return float4(result, 1.0f);
}
