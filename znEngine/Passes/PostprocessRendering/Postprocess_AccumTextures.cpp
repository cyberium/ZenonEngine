#include "stdafx.h"

// General
#include "Postprocess_AccumTextures.h"

CPassPostprocess_AccumTextures::CPassPostprocess_AccumTextures(IRenderDevice& RenderDevice, std::vector<std::shared_ptr<ITexture>> InputTextures)
	: RenderPassPipelined(RenderDevice)
	, m_InputTextures(InputTextures)
{}

CPassPostprocess_AccumTextures::~CPassPostprocess_AccumTextures()
{}



std::shared_ptr<ITexture> CPassPostprocess_AccumTextures::GetOutputTexture() const
{
	return m_OutputTexture;
}



//
// IRenderPass
//
void CPassPostprocess_AccumTextures::Render(RenderEventArgs& e)
{
	GetPipeline().GetRenderTarget()->Clear();

	for (const auto& texture : m_InputTextures)
	{
		texture->Bind(0, GetPipeline().GetPixelShaderPtr(), IShaderParameter::EType::Texture);
		{
			m_QuadGeometry->Render(GetPipeline().GetVertexShaderPtr());
		}
		texture->UnBind(0, GetPipeline().GetPixelShaderPtr(), IShaderParameter::EType::Texture);
	}
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassPostprocess_AccumTextures::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	m_QuadGeometry = GetRenderDevice().GetPrimitivesFactory().CreateQuad();

	auto samplesCnt = std::to_string(GetPipeline().GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0)->GetSamplesCount());

	auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/PostprocessCopyTexture.hlsl", "VS_ScreenQuad", { {"MULTISAMPLED", samplesCnt.c_str() } });
	vertexShader->LoadInputLayoutFromReflector();
	GetPipeline().SetShader(vertexShader);

	auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/PostprocessCopyTexture.hlsl", "PS_ScreenQuad", { {"MULTISAMPLED", samplesCnt.c_str() } });
	GetPipeline().SetShader(pixelShader);

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(additiveBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	GetPipeline().SetRenderTarget(CreateRenderTarget(RenderTarget));

	m_OutputTexture = GetPipeline().GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0);

	return shared_from_this();
}



//
// Private
//
std::shared_ptr<IRenderTarget> CPassPostprocess_AccumTextures::CreateRenderTarget(std::shared_ptr<IRenderTarget> RenderTarget)
{
	ITexture::TextureFormat colorTextureFormat
	(
		ITexture::Components::RGBA,
		ITexture::Type::Float,
		RenderTarget->GetSamplesCount(),
		32, 32, 32, 32, 0, 0
	);
	auto outputTexture = GetRenderDevice().GetObjectsFactory().CreateTexture2D(RenderTarget->GetViewport().GetWidth(), RenderTarget->GetViewport().GetHeight(), 1, colorTextureFormat);

	auto rt = GetRenderDevice().GetObjectsFactory().CreateRenderTarget();
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color0, outputTexture);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, RenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));
	return rt;
}

void CPassPostprocess_AccumTextures::SetOutputTexture(std::shared_ptr<ITexture> Texture)
{
	m_OutputTexture = Texture;
}