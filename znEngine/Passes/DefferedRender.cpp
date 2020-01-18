#include "stdafx.h"

// General
#include "DefferedRender.h"

CDefferedRender::CDefferedRender(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<BuildRenderListPass> BuildRenderListPass)
	: RenderListProcessorPass(RenderDevice, BuildRenderListPass)
{
	m_PerObjectData = (PerObject3D*)_aligned_malloc(sizeof(PerObject3D), 16);
	m_PerObjectConstantBuffer = GetRenderDevice()->CreateConstantBuffer(PerObject3D());
}

CDefferedRender::~CDefferedRender()
{
	_aligned_free(m_PerObjectData);
	GetRenderDevice()->DestroyConstantBuffer(m_PerObjectConstantBuffer);
}


//
// CDefferedRender
//
std::shared_ptr<ITexture> CDefferedRender::GetTexture0() const
{
	return m_Texture0;
}

std::shared_ptr<ITexture> CDefferedRender::GetTexture1() const
{
	return m_Texture1;
}

std::shared_ptr<ITexture> CDefferedRender::GetTexture2() const
{
	return m_Texture2;
}

std::shared_ptr<ITexture> CDefferedRender::GetTexture3() const
{
	return m_Texture3;
}

std::shared_ptr<ITexture> CDefferedRender::GetTextureDepthStencil() const
{
	return m_DepthStencilTexture;
}


void CDefferedRender::PreRender(RenderEventArgs & e)
{
	RenderListProcessorPass::PreRender(e);

	GetPipeline()->GetRenderTarget()->Clear(ClearFlags::All, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
}




//
// IRenderPassPipelined
//
void CDefferedRender::CreatePipeline(std::shared_ptr<IRenderTarget> /*RenderTarget*/, const Viewport * Viewport)
{
	ITexture::TextureFormat colorTextureFormat
	(
		ITexture::Components::RGBA,
		ITexture::Type::UnsignedNormalized,
		1,
		8, 8, 8, 8, 0, 0
	);
	ITexture::TextureFormat positionTextureFormat
	(
		ITexture::Components::RGBA,
		ITexture::Type::Float,
		1,
		32, 32, 32, 32, 0, 0
	);
	m_Texture0 = GetRenderDevice()->CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, colorTextureFormat);
	m_Texture1 = GetRenderDevice()->CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, colorTextureFormat);
	m_Texture2 = GetRenderDevice()->CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, positionTextureFormat);
	m_Texture3 = GetRenderDevice()->CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, positionTextureFormat);

	// Depth/stencil buffer
	ITexture::TextureFormat depthStencilTextureFormat(
		ITexture::Components::DepthStencil,
		ITexture::Type::UnsignedNormalized,
		1,
		0, 0, 0, 0, 24, 8);
	m_DepthStencilTexture = GetRenderDevice()->CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, depthStencilTextureFormat);

	std::shared_ptr<IRenderTarget> rt = GetRenderDevice()->CreateRenderTarget();
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color0, m_Texture0);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color1, m_Texture1);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color2, m_Texture2);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color3, m_Texture3);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, m_DepthStencilTexture);

	std::shared_ptr<IShader> vertexShader = GetRenderDevice()->CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_MODEL_DEFFERED", IShader::ShaderMacros(), "VS_PTN", "latest");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice()->CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_MODEL_DEFFERED", IShader::ShaderMacros(), "PS_main", "latest");

	// PIPELINES
	std::shared_ptr<IPipelineState> defferedPipeline = GetRenderDevice()->CreatePipelineState();
	defferedPipeline->GetBlendState()->SetBlendMode(disableBlending);
	defferedPipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	defferedPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	defferedPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	defferedPipeline->SetRenderTarget(rt);
	defferedPipeline->GetRasterizerState()->SetViewport(Viewport);
	defferedPipeline->SetShader(EShaderType::VertexShader, vertexShader);
	defferedPipeline->SetShader(EShaderType::PixelShader, pixelShader);

	std::shared_ptr<ISamplerState> sampler = GetRenderDevice()->CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	defferedPipeline->SetSampler(0, sampler);

	std::shared_ptr<ISamplerState> samplerClamp = GetRenderDevice()->CreateSamplerState();
	samplerClamp->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	samplerClamp->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	defferedPipeline->SetSampler(1, samplerClamp);

	SetPipeline(defferedPipeline);
}

void CDefferedRender::UpdateViewport(const Viewport * _viewport)
{
	RenderListProcessorPass::UpdateViewport(_viewport);

	GetPipeline()->GetRenderTarget()->Resize(_viewport->GetWidth(), _viewport->GetHeight());
}



//
// IVisitor
//
bool CDefferedRender::Visit3D(ISceneNode * node)
{
	RenderListProcessorPass::VisitBase(node);

	const ICamera* camera = GetRenderEventArgs()->Camera;
	//const Viewport* viewport = GetRenderEventArgs()->PipelineState->GetRasterizerState()->GetViewports()[0];

	if (camera)
	{
		m_PerObjectData->Model = node->GetWorldTransfom();
		m_PerObjectConstantBuffer->Set(m_PerObjectData, sizeof(PerObject3D));

		return true;
	}

	return false;
}

bool CDefferedRender::Visit(IGeometry * Geometry, const IMaterial * Material, SGeometryPartParams GeometryPartParams)
{
	GetRenderEventArgs()->Caller = this;

	ShaderMap shadersMap;

	if (Material)
		shadersMap = Material->GetShaders();

	if (shadersMap.empty())
		shadersMap = GetRenderEventArgs()->PipelineState->GetShaders();

	Material->Bind(shadersMap);
	bool result = Geometry->Render(GetRenderEventArgs(), m_PerObjectConstantBuffer.get(), shadersMap, Material, GeometryPartParams);
	Material->Unbind(shadersMap);

	return result;
}

bool CDefferedRender::Visit(ILightComponent3D * light)
{
	return false;
}
