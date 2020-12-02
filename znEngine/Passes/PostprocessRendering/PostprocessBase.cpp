#include "stdafx.h"

// General
#include "PostprocessBase.h"

CPassPostprocessBase::CPassPostprocessBase(IRenderDevice& RenderDevice, std::shared_ptr<ITexture> InputTexture)
	: RenderPassPipelined(RenderDevice)
	, m_InputTexture(InputTexture)
	, m_IsCustomRenderTarget(false)
{}

CPassPostprocessBase::~CPassPostprocessBase()
{}



std::shared_ptr<ITexture> CPassPostprocessBase::GetOutputTexture() const
{
	return m_OutputTexture;
}



//
// IRenderPass
//
void CPassPostprocessBase::Render(RenderEventArgs& e)
{
	if (m_IsCustomRenderTarget)
		GetPipeline().GetRenderTarget()->Clear();

	m_QuadGeometry->Render(GetPipeline().GetVertexShaderPtr());
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassPostprocessBase::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	m_QuadGeometry = GetRenderDevice().GetPrimitivesFactory().CreateQuad();

	GetPipeline().SetShader(EShaderType::VertexShader, LoadVertexShader());
	GetPipeline().SetShader(EShaderType::PixelShader, LoadPixelShader());

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(additiveBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	GetPipeline().SetRenderTarget(LoadRenderTarget(RenderTarget));
	
	GetPipeline().SetTexture(0, m_InputTexture);

	return shared_from_this();
}



//
// Private
//
std::shared_ptr<IRenderTarget> CPassPostprocessBase::CreateRenderTarget(std::shared_ptr<IRenderTarget> RenderTarget)
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

	m_IsCustomRenderTarget = true;

	return rt;
}

void CPassPostprocessBase::SetOutputTexture(std::shared_ptr<ITexture> Texture)
{
	m_OutputTexture = Texture;
}
