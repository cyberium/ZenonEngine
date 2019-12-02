/*struct PixelShaderOutput
{
	float4 PositionWS           : SV_Target0;   // Position in View space (R8G8B8_UNORM) MaterialType (A8_UNORM)            
	float4 Diffuse              : SV_Target1;   // Diffuse Albedo (R8G8B8_UNORM) Unused (A8_UNORM)
    float4 Specular             : SV_Target2;   // Specular Color (R8G8B8_UNROM) Specular Power(A8_UNORM)
    float4 NormalWS             : SV_Target3;   // View space normal (R32G32B32_FLOAT) Unused (A32_FLOAT)
};*/


struct PixelShaderOutput
{
    float4 Diffuse              : SV_Target0;   // Diffuse Albedo (R8G8B8_UNORM) Unused (A8_UNORM)
    float4 PositionWS           : SV_Target1;   // Position in View space (R8G8B8_UNORM) MaterialType (A8_UNORM)            
    float4 Specular             : SV_Target2;   // Specular Color (R8G8B8_UNROM) Specular Power(A8_UNORM)
    float4 NormalWS             : SV_Target3;   // View space normal (R32G32B32_FLOAT) Unused (A32_FLOAT)
};