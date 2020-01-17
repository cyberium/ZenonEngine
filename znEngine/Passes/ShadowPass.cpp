#include "stdafx.h"

// General
#include "ShadowPass.h"

CShadowPass::CShadowPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene)
	: Base3DPass(RenderDevice, Scene, nullptr)
{
	CreatePipeline();

	m_PerLightData = (PerLight*)_aligned_malloc(sizeof(PerLight), 16);
	m_PerLightConstantBuffer = GetRenderDevice()->CreateConstantBuffer(PerLight());
}

CShadowPass::~CShadowPass()
{
	_aligned_free(m_PerObjectData);
	GetRenderDevice()->DestroyConstantBuffer(m_PerObjectConstantBuffer);
}

void CShadowPass::PreRender(RenderEventArgs & e)
{
	Base3DPass::PreRender(e);

	m_ShadowPipeline->GetRenderTarget()->Clear(ClearFlags::All, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
}

void CShadowPass::Render(RenderEventArgs & e)
{
	m_ShadowPipeline->Bind();
	CBaseScenePass::Render(e);
	m_ShadowPipeline->UnBind();
}

void CShadowPass::PostRender(RenderEventArgs & e)
{
	Base3DPass::PostRender(e);
}

bool CShadowPass::Visit3D(ISceneNode * node)
{
	CBaseScenePass::VisitBase(node);

	m_PerObjectData->Model = node->GetWorldTransfom();
	//m_PerObjectData->View = glm::mat4(1.0f);
	//m_PerObjectData->Projection = glm::mat4(1.0f);

	SetPerObjectConstantBufferData();

	return true;
}

bool CShadowPass::Visit(IGeometry * Geometry, const IMaterial * Material, SGeometryPartParams GeometryPartParams)
{
	return Base3DPass::Visit(Geometry, Material, GeometryPartParams);
}

bool CShadowPass::Visit(ILightComponent3D * light)
{
	m_PerLightData->LightView = light->GetViewMatrix();
	m_PerLightData->LightProjection = light->GetProjectionMatrix();
	m_PerLightConstantBuffer->Set(m_PerLightData, sizeof(PerLight));

	m_PerObjectData->View = light->GetViewMatrix();
	m_PerObjectData->Projection = light->GetProjectionMatrix();

	SetPerObjectConstantBufferData();

	return true;
}

std::shared_ptr<ITexture> CShadowPass::GetColorTexture() const
{
	return m_ColorTexture;
}

std::shared_ptr<ITexture> CShadowPass::GetShadowTexture() const
{
	return m_ShadowTexture;
}

std::shared_ptr<IConstantBuffer> CShadowPass::GetPerLightBuffer() const
{
	return m_PerLightConstantBuffer;
}

void CShadowPass::CreatePipeline()
{
	const float size = 2048.0f;
	ITexture::TextureFormat colorTextureFormat
	(
		ITexture::Components::R,
		ITexture::Type::Float,
		1,
		32, 0, 0, 0, 0, 0
	);
	m_ColorTexture = GetRenderDevice()->CreateTexture2D(size, size, 1, colorTextureFormat);

	// Depth/stencil buffer
	ITexture::TextureFormat depthStencilTextureFormat(
		ITexture::Components::DepthStencil,
		ITexture::Type::UnsignedNormalized,
		1,
		0, 0, 0, 0, 24, 8);
	m_ShadowTexture = GetRenderDevice()->CreateTexture2D(size, size, 1, depthStencilTextureFormat);


	std::shared_ptr<IRenderTarget> rt = GetRenderDevice()->CreateRenderTarget();
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color0, m_ColorTexture);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, m_ShadowTexture);

	
	v.SetWidth(size);
	v.SetHeight(size);

	IBlendState::BlendMode alphaBlending(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
	IBlendState::BlendMode disableBlending;
	IDepthStencilState::DepthMode enableDepthWrites(true, IDepthStencilState::DepthWrite::Enable);
	IDepthStencilState::DepthMode disableDepthWrites(false, IDepthStencilState::DepthWrite::Disable);

	std::shared_ptr<IShader> vertexShader = GetRenderDevice()->CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_SHADOW", IShader::ShaderMacros(), "VS_Shadow", "latest");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice()->CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_SHADOW", IShader::ShaderMacros(), "PS_Shadow", "latest");

	// PIPELINES
	m_ShadowPipeline = GetRenderDevice()->CreatePipelineState();
	m_ShadowPipeline->GetBlendState()->SetBlendMode(disableBlending);
	m_ShadowPipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	m_ShadowPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	m_ShadowPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	m_ShadowPipeline->SetRenderTarget(rt);
	m_ShadowPipeline->GetRasterizerState()->SetViewport(&v);
	m_ShadowPipeline->SetShader(EShaderType::VertexShader, vertexShader);
	m_ShadowPipeline->SetShader(EShaderType::PixelShader, pixelShader);
}
