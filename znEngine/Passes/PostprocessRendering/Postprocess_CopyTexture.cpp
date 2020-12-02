#include "stdafx.h"

// General
#include "Postprocess_CopyTexture.h"

CPassPostprocess_CopyTexture::CPassPostprocess_CopyTexture(IRenderDevice& RenderDevice, std::shared_ptr<ITexture> InputTexture)
	: CPassPostprocessBase(RenderDevice, InputTexture)
{}

CPassPostprocess_CopyTexture::~CPassPostprocess_CopyTexture()
{}


std::shared_ptr<IRenderTarget> CPassPostprocess_CopyTexture::LoadRenderTarget(std::shared_ptr<IRenderTarget> OriginalRenderTarget)
{
	SetOutputTexture(OriginalRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	return OriginalRenderTarget;
}

std::shared_ptr<IShader> CPassPostprocess_CopyTexture::LoadVertexShader()
{
	auto samplesCnt = std::to_string(GetPipeline().GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0)->GetSamplesCount());

	auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/PostprocessCopyTexture.hlsl", "VS_ScreenQuad", { {"MULTISAMPLED", samplesCnt.c_str() } });
	vertexShader->LoadInputLayoutFromReflector();
	return vertexShader;
}

std::shared_ptr<IShader> CPassPostprocess_CopyTexture::LoadPixelShader()
{
	auto samplesCnt = std::to_string(GetPipeline().GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0)->GetSamplesCount());

	auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/PostprocessCopyTexture.hlsl", "PS_ScreenQuad", { {"MULTISAMPLED", samplesCnt.c_str() } });
	return pixelShader;
}