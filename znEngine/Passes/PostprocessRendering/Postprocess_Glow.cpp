#include "stdafx.h"

// General
#include "Postprocess_Glow.h"

CPassPostprocess_Glow::CPassPostprocess_Glow(IRenderDevice& RenderDevice, std::shared_ptr<ITexture> InputTexture)
	: CPassPostprocessBase(RenderDevice, InputTexture)
{}

CPassPostprocess_Glow::~CPassPostprocess_Glow()
{}

std::shared_ptr<IRenderTarget> CPassPostprocess_Glow::LoadRenderTarget(std::shared_ptr<IRenderTarget> OriginalRenderTarget)
{
	auto newRenderTarget = CreateRenderTarget(OriginalRenderTarget);
	SetOutputTexture(newRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	return newRenderTarget;
}

std::shared_ptr<IShader> CPassPostprocess_Glow::LoadVertexShader()
{
	auto samplesCnt = std::to_string(GetPipeline().GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0)->GetSamplesCount());

	auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/PostprocessGlow.hlsl", "VS_ScreenQuad", { {"MULTISAMPLED", samplesCnt.c_str() } });
	vertexShader->LoadInputLayoutFromReflector();
	return vertexShader;
}

std::shared_ptr<IShader> CPassPostprocess_Glow::LoadPixelShader()
{
	auto samplesCnt = std::to_string(GetPipeline().GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0)->GetSamplesCount());

	auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/PostprocessGlow.hlsl", "PS_ScreenQuad", { {"MULTISAMPLED", samplesCnt.c_str() } });
	return pixelShader;
}
