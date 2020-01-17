#pragma once

enum class ZN_API EShaderType
{
	UnknownShaderType = 0,
	VertexShader,
	TessellationControlShader,      // Hull Shader in DirectX
	TessellationEvaluationShader,   // Domain Shader in DirectX
	GeometryShader,
	PixelShader,
	ComputeShader,
};