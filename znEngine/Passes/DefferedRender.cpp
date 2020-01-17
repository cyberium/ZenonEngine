#include "stdafx.h"

// General
#include "DefferedRender.h"

CDefferedRender::CDefferedRender(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene)
	: CBaseScenePass(RenderDevice, Scene)
{
	m_PerObjectData = (PerObject3D*)_aligned_malloc(sizeof(PerObject3D), 16);
	m_PerObjectConstantBuffer = GetRenderDevice()->CreateConstantBuffer(PerObject3D());

	CreatePipeline();
}

CDefferedRender::~CDefferedRender()
{
	_aligned_free(m_PerObjectData);
}

void CDefferedRender::PreRender(RenderEventArgs & e)
{
	e.PipelineState = m_DefferedPipeline.get();

	CBaseScenePass::PreRender(e);

	m_DefferedPipeline->GetRenderTarget()->Clear(ClearFlags::All, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
}

void CDefferedRender::Render(RenderEventArgs & e)
{
	m_DefferedPipeline->Bind();
	CBaseScenePass::Render(e);
	m_DefferedPipeline->UnBind();
}

void CDefferedRender::PostRender(RenderEventArgs & e)
{
	CBaseScenePass::PostRender(e);
}

void CDefferedRender::UpdateViewport(const Viewport * _viewport)
{
	m_DefferedPipeline->GetRasterizerState()->SetViewport(_viewport);
}

bool CDefferedRender::Visit3D(ISceneNode * node)
{
	CBaseScenePass::VisitBase(node);

	const ICamera* camera = GetRenderEventArgs()->Camera;
	//const Viewport* viewport = GetRenderEventArgs()->PipelineState->GetRasterizerState()->GetViewports()[0];

	if (camera)
	{
		m_PerObjectData->Model = node->GetWorldTransfom();
		m_PerObjectData->View = camera->GetViewMatrix();
		m_PerObjectData->Projection = camera->GetProjectionMatrix();
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



//
// Protected
//
void CDefferedRender::CreatePipeline()
{
	const float size = 2048.0f;

	ITexture::TextureFormat colorTextureFormat
	(
		ITexture::Components::RGBA,
		ITexture::Type::UnsignedNormalized,
		1,
		8, 8, 8, 8, 0, 0
	);
	m_Texture0 = GetRenderDevice()->CreateTexture2D(size, size, 1, colorTextureFormat);
	m_Texture1 = GetRenderDevice()->CreateTexture2D(size, size, 1, colorTextureFormat);
	m_Texture2 = GetRenderDevice()->CreateTexture2D(size, size, 1, colorTextureFormat);
	m_Texture3 = GetRenderDevice()->CreateTexture2D(size, size, 1, colorTextureFormat);

	// Depth/stencil buffer
	ITexture::TextureFormat depthStencilTextureFormat(
		ITexture::Components::DepthStencil,
		ITexture::Type::UnsignedNormalized,
		1,
		0, 0, 0, 0, 24, 8);
	m_DepthStencilTexture = GetRenderDevice()->CreateTexture2D(size, size, 1, depthStencilTextureFormat);


	std::shared_ptr<IRenderTarget> rt = GetRenderDevice()->CreateRenderTarget();
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color0, m_Texture0);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color1, m_Texture1);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color2, m_Texture2);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color3, m_Texture3);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, m_DepthStencilTexture);


	v.SetWidth(size);
	v.SetHeight(size);

	IBlendState::BlendMode alphaBlending(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
	IBlendState::BlendMode disableBlending;
	IDepthStencilState::DepthMode enableDepthWrites(true, IDepthStencilState::DepthWrite::Enable);
	IDepthStencilState::DepthMode disableDepthWrites(false, IDepthStencilState::DepthWrite::Disable);

	std::shared_ptr<IShader> vertexShader = GetRenderDevice()->CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_MODEL_DEFFERED", IShader::ShaderMacros(), "VS_PTN", "latest");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice()->CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_MODEL_DEFFERED", IShader::ShaderMacros(), "PS_main", "latest");

	// PIPELINES
	m_DefferedPipeline = GetRenderDevice()->CreatePipelineState();
	m_DefferedPipeline->GetBlendState()->SetBlendMode(disableBlending);
	m_DefferedPipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	m_DefferedPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	m_DefferedPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	m_DefferedPipeline->SetRenderTarget(rt);
	m_DefferedPipeline->GetRasterizerState()->SetViewport(&v);
	m_DefferedPipeline->SetShader(EShaderType::VertexShader, vertexShader);
	m_DefferedPipeline->SetShader(EShaderType::PixelShader, pixelShader);
}
