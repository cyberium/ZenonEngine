#include "stdafx.h"

// General
#include "ShadowPass.h"

CShadowPass::CShadowPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene)
	: Base3DPass(RenderDevice, Scene)
{
	m_PerLightData = (PerLight*)_aligned_malloc(sizeof(PerLight), 16);
	m_PerLightConstantBuffer = GetRenderDevice()->CreateConstantBuffer(PerLight());
}

CShadowPass::~CShadowPass()
{
	_aligned_free(m_PerObjectData);
	GetRenderDevice()->DestroyConstantBuffer(m_PerObjectConstantBuffer);
}



//
// CShadowPass
//
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



//
// IRenderPass
//
void CShadowPass::PreRender(RenderEventArgs & e)
{
	Base3DPass::PreRender(e);

	m_RenderTarget->Clear(ClearFlags::All, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
}



//
// IRenderPassPipelined
//
void CShadowPass::CreatePipeline(std::shared_ptr<IRenderTarget> /*RenderTarget*/, const Viewport * /*Viewport*/)
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


	m_RenderTarget = GetRenderDevice()->CreateRenderTarget();
	m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color0, m_ColorTexture);
	m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, m_ShadowTexture);


	v.SetWidth(size);
	v.SetHeight(size);

	std::shared_ptr<IShader> vertexShader = GetRenderDevice()->CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_SHADOW", IShader::ShaderMacros(), "VS_Shadow", "latest");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice()->CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_SHADOW", IShader::ShaderMacros(), "PS_Shadow", "latest");

	// PIPELINES
	std::shared_ptr<IPipelineState> shadowPipeline = GetRenderDevice()->CreatePipelineState();
	shadowPipeline->GetBlendState()->SetBlendMode(disableBlending);
	shadowPipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	shadowPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	shadowPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	shadowPipeline->SetRenderTarget(m_RenderTarget);
	shadowPipeline->GetRasterizerState()->SetViewport(&v);
	shadowPipeline->SetShader(EShaderType::VertexShader, vertexShader);
	shadowPipeline->SetShader(EShaderType::PixelShader, pixelShader);

	SetPipeline(shadowPipeline);
}



//
// IVisitor
//
bool CShadowPass::Visit3D(ISceneNode * node)
{
	Base3DPass::VisitBase(node);

	m_PerObjectData->Model = node->GetWorldTransfom();
	//m_PerObjectData->View = glm::mat4(1.0f);
	//m_PerObjectData->Projection = glm::mat4(1.0f);
	m_PerObjectConstantBuffer->Set(*m_PerObjectData);

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
	m_PerLightConstantBuffer->Set(*m_PerLightData);

	m_PerObjectData->View = light->GetViewMatrix();
	m_PerObjectData->Projection = light->GetProjectionMatrix();
	m_PerObjectConstantBuffer->Set(*m_PerObjectData);

	return true;
}
