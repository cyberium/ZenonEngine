#include "stdafx.h"

// General
#include "Postprocess_ApplyTexture.h"

CPassPostprocess_ApplyTexture::CPassPostprocess_ApplyTexture(IRenderDevice& RenderDevice, std::shared_ptr<ITexture> InputTexture)
	: CPassPostprocessBase(RenderDevice, InputTexture)
{}

CPassPostprocess_ApplyTexture::~CPassPostprocess_ApplyTexture()
{}


std::shared_ptr<IRenderTarget> CPassPostprocess_ApplyTexture::LoadRenderTarget(std::shared_ptr<IRenderTarget> OriginalRenderTarget)
{
	SetOutputTexture(OriginalRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	return OriginalRenderTarget;
}

std::shared_ptr<IShader> CPassPostprocess_ApplyTexture::LoadVertexShader()
{
	auto samplesCnt = std::to_string(GetPipeline().GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0)->GetSamplesCount());

	auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/PostprocessCopyTexture.hlsl", "VS_ScreenQuad", { {"MULTISAMPLED", samplesCnt.c_str() } });
	vertexShader->LoadInputLayoutFromReflector();
	return vertexShader;
}

std::shared_ptr<IShader> CPassPostprocess_ApplyTexture::LoadPixelShader()
{
	auto samplesCnt = std::to_string(GetPipeline().GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0)->GetSamplesCount());

	auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/PostprocessCopyTexture.hlsl", "PS_ScreenQuad", { {"MULTISAMPLED", samplesCnt.c_str() } });
	return pixelShader;
}