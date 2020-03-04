#include "stdafx.h"

// General
#include "DefferedRender.h"

CDefferedRender::CDefferedRender(IRenderDevice& RenderDevice, const std::shared_ptr<CSceneCreateTypelessListPass>& SceneCreateTypelessListPass)
	: RenderPassPipelined(RenderDevice)
	, m_SceneCreateTypelessListPass(SceneCreateTypelessListPass)
{
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject3D());
}

CDefferedRender::~CDefferedRender()
{
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

//
// IRenderPass
//
void CDefferedRender::PreRender(RenderEventArgs & e)
{
	__super::PreRender(e);

	GetPipeline().GetRenderTarget()->Clear(ClearFlags::All, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
}

void CDefferedRender::Render(RenderEventArgs& e)
{
	for (const auto& it : m_SceneCreateTypelessListPass->GetGeometryList())
	{
		Visit(const_cast<ISceneNode3D*>(it.Node)); // TODO!!!
		Visit(const_cast<IGeometry*>(it.Geometry), it.Material, it.GeometryDrawArgs);
	}

	for (const auto& it : m_SceneCreateTypelessListPass->GetLightList())
	{
		Visit(const_cast<ISceneNode3D*>(it.SceneNode)); // TODO!!!
		Visit(const_cast<ILightComponent3D*>(it.Light));
	}
}

//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CDefferedRender::CreatePipeline(std::shared_ptr<IRenderTarget> /*RenderTarget*/, const Viewport * Viewport)
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
	m_Texture0 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, colorTextureFormat);
	m_Texture1 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, colorTextureFormat);
	//m_Texture2 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, positionTextureFormat);
	m_Texture3 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, positionTextureFormat);

	// Depth/stencil buffer
	ITexture::TextureFormat depthStencilTextureFormat(
		ITexture::Components::DepthStencil,
		ITexture::Type::UnsignedNormalized,
		1,
		0, 0, 0, 0, 24, 8);
	m_DepthStencilTexture = GetRenderDevice().GetObjectsFactory().CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, depthStencilTextureFormat);

	auto rt = GetRenderDevice().GetObjectsFactory().CreateRenderTarget();
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color0, m_Texture0);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color1, m_Texture1);
	//rt->AttachTexture(IRenderTarget::AttachmentPoint::Color2, m_Texture2);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color3, m_Texture3);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, m_DepthStencilTexture);

	auto vertexShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_MODEL_DEFFERED", "VS_PTN");
	vertexShader->LoadInputLayoutFromReflector();

	m_PerObjectShaderParameter = &vertexShader->GetShaderParameterByName("PerObject");
	_ASSERT(m_PerObjectShaderParameter->IsValid());
	m_PerObjectShaderParameter->SetConstantBuffer(m_PerObjectConstantBuffer);

	auto pixelShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_MODEL_DEFFERED", "PS_main");

	// PIPELINES
	auto defferedPipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
	defferedPipeline->GetBlendState()->SetBlendMode(disableBlending);
	defferedPipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	defferedPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	defferedPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	defferedPipeline->SetRenderTarget(rt);
	defferedPipeline->GetRasterizerState()->SetViewport(Viewport);
	defferedPipeline->SetShader(EShaderType::VertexShader, vertexShader);
	defferedPipeline->SetShader(EShaderType::PixelShader, pixelShader);

	auto sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	defferedPipeline->SetSampler(0, sampler);

	auto samplerClamp = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	samplerClamp->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	samplerClamp->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	defferedPipeline->SetSampler(1, samplerClamp);

	return SetPipeline(defferedPipeline);
}

void CDefferedRender::UpdateViewport(const Viewport& _viewport)
{
	__super::UpdateViewport(_viewport);

	GetPipeline().GetRenderTarget()->Resize(_viewport.GetWidth(), _viewport.GetHeight());
}



//
// IVisitor
//
bool CDefferedRender::Visit(const ISceneNode3D * node)
{
	__super::Visit(node);

	PerObject3D perObject3D;
	perObject3D.Model = node->GetWorldTransfom();
	m_PerObjectConstantBuffer->Set(perObject3D);
	m_PerObjectShaderParameter->Bind();

	return true;
}

bool CDefferedRender::Visit(const IGeometry * Geometry, const IMaterial * Material, SGeometryDrawArgs GeometryDrawArgs)
{
	const auto& shaders = GetRenderEventArgs().PipelineState->GetShaders();

	Material->Bind(shaders);
	Geometry->Render(GetRenderEventArgs(), shaders.at(EShaderType::VertexShader).get(), GeometryDrawArgs);
	Material->Unbind(shaders);

	return true;
}

bool CDefferedRender::Visit(const ILightComponent3D * light)
{
	return false;
}
