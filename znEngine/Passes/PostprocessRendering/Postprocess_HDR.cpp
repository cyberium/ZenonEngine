#include "stdafx.h"

// General
#include "Postprocess_HDR.h"

CPassPostprocess_HDR::CPassPostprocess_HDR(IRenderDevice& RenderDevice, std::shared_ptr<ITexture> InputTexture)
	: CPassPostprocessBase(RenderDevice, InputTexture)
{}

CPassPostprocess_HDR::~CPassPostprocess_HDR()
{}

std::shared_ptr<IRenderTarget> CPassPostprocess_HDR::LoadRenderTarget(std::shared_ptr<IRenderTarget> OriginalRenderTarget)
{
	auto newRenderTarget = CreateRenderTarget(OriginalRenderTarget);
	SetOutputTexture(newRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	return newRenderTarget;
}

std::shared_ptr<IShader> CPassPostprocess_HDR::LoadVertexShader()
{
	auto samplesCnt = std::to_string(GetPipeline().GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0)->GetSamplesCount());

	auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/PostprocessHDR.hlsl", "VS_ScreenQuad", { {"MULTISAMPLED", samplesCnt.c_str() } });
	vertexShader->LoadInputLayoutFromReflector();
	return vertexShader;
}

std::shared_ptr<IShader> CPassPostprocess_HDR::LoadPixelShader()
{
	auto samplesCnt = std::to_string(GetPipeline().GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0)->GetSamplesCount());

	auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/PostprocessHDR.hlsl", "PS_ScreenQuad", { {"MULTISAMPLED", samplesCnt.c_str() } });
	return pixelShader;
}
