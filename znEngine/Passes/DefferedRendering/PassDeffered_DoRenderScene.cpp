#include "stdafx.h"

// General
#include "PassDeffered_DoRenderScene.h"

// Additional
#include "Materials/MaterialModel.h"

CPassDeffered_DoRenderScene::CPassDeffered_DoRenderScene(IRenderDevice& RenderDevice, const std::shared_ptr<CSceneCreateTypelessListPass>& SceneCreateTypelessListPass)
	: RenderPassPipelined(RenderDevice)
	, m_SceneCreateTypelessListPass(SceneCreateTypelessListPass)
{
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject());
}

CPassDeffered_DoRenderScene::~CPassDeffered_DoRenderScene()
{
}


//
// CPassDeffered_DoRenderScene
//
std::shared_ptr<ITexture> CPassDeffered_DoRenderScene::GetTexture0() const
{
	return m_Texture0;
}

std::shared_ptr<ITexture> CPassDeffered_DoRenderScene::GetTexture1() const
{
	return m_Texture1;
}

std::shared_ptr<ITexture> CPassDeffered_DoRenderScene::GetTexture2() const
{
	return m_Texture2;
}

std::shared_ptr<ITexture> CPassDeffered_DoRenderScene::GetTexture3() const
{
	return m_Texture3;
}

std::shared_ptr<ITexture> CPassDeffered_DoRenderScene::GetTextureDepthStencil() const
{
	return m_DepthStencilTexture;
}

//
// IRenderPass
//
void CPassDeffered_DoRenderScene::PreRender(RenderEventArgs & e)
{
	__super::PreRender(e);

	GetPipeline().GetRenderTarget()->Clear(ClearFlags::All, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
}

void CPassDeffered_DoRenderScene::Render(RenderEventArgs& e)
{
	for (const auto& it : m_SceneCreateTypelessListPass->GetGeometryList())
	{
		if (dynamic_cast<const MaterialModel*>(it.Material))
		{
			Visit(it.Node);
			Visit(it.Geometry, it.Material, it.GeometryDrawArgs);
		}
	}

	for (const auto& it : m_SceneCreateTypelessListPass->GetLightList())
	{
		Visit(it.SceneNode);
		Visit(it.Light);
	}
}

//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassDeffered_DoRenderScene::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Model_Deffered.hlsl", "VS_PTN");
	vertexShader->LoadInputLayoutFromReflector();

	m_PerObjectShaderParameter = &vertexShader->GetShaderParameterByName("PerObject");
	_ASSERT(m_PerObjectShaderParameter->IsValid());
	m_PerObjectShaderParameter->SetConstantBuffer(m_PerObjectConstantBuffer);

	auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Model_Deffered.hlsl", "PS_main");

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(disableBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	//GetPipeline().GetRasterizerState()->SetAntialiasedLineEnable(true);
	//GetPipeline().GetRasterizerState()->SetMultisampleEnabled(true);
	GetPipeline().SetRenderTarget(CreateGBuffer(RenderTarget, Viewport));
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	auto sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	GetPipeline().SetSampler(0, sampler);

	auto samplerClamp = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	samplerClamp->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	samplerClamp->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	GetPipeline().SetSampler(1, samplerClamp);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CPassDeffered_DoRenderScene::Visit(const ISceneNode * node)
{
	if (node->GetType() != cSceneNode3D)
		return EVisitResult::AllowVisitChilds;

	PerObject perObject;
	perObject.Model = node->GetWorldTransfom();
	m_PerObjectConstantBuffer->Set(perObject);
	m_PerObjectShaderParameter->Bind();

	return EVisitResult::AllowAll;
}

EVisitResult CPassDeffered_DoRenderScene::Visit(const IGeometry * Geometry, const IMaterial * Material, SGeometryDrawArgs GeometryDrawArgs)
{
	const auto& shaders = GetRenderEventArgs().PipelineState->GetShaders();

	Material->Bind(shaders);
	Geometry->Render(GetRenderEventArgs(), shaders.at(EShaderType::VertexShader).get(), GeometryDrawArgs);
	Material->Unbind(shaders);

	return EVisitResult::AllowAll;
}

EVisitResult CPassDeffered_DoRenderScene::Visit(const ILight3D * light)
{
	return EVisitResult::AllowAll;
}



//
// Private
//
std::shared_ptr<IRenderTarget> CPassDeffered_DoRenderScene::CreateGBuffer(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	ITexture::TextureFormat colorTextureFormat
	(
		ITexture::Components::RGBA,
		ITexture::Type::UnsignedNormalized,
		RenderTarget->GetSamplesCount(),
		8, 8, 8, 8, 0, 0
	);
	ITexture::TextureFormat normalTextureFormat
	(
		ITexture::Components::RGBA,
		ITexture::Type::Float,
		RenderTarget->GetSamplesCount(),
		32, 32, 32, 32, 0, 0
	);
	ITexture::TextureFormat positionTextureFormat
	(
		ITexture::Components::RGBA,
		ITexture::Type::Float,
		RenderTarget->GetSamplesCount(),
		32, 32, 32, 32, 0, 0
	);
	m_Texture0 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, colorTextureFormat);
	m_Texture1 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, colorTextureFormat);
	m_Texture2 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, normalTextureFormat);
	m_Texture3 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, positionTextureFormat);

	// Depth/stencil buffer
	ITexture::TextureFormat depthStencilTextureFormat(
		ITexture::Components::DepthStencil,
		ITexture::Type::UnsignedNormalized,
		RenderTarget->GetSamplesCount(),
		0, 0, 0, 0, 24, 8);
	m_DepthStencilTexture = GetRenderDevice().GetObjectsFactory().CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, depthStencilTextureFormat);

	auto rt = GetRenderDevice().GetObjectsFactory().CreateRenderTarget();
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color0, m_Texture0);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color1, m_Texture1);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color2, m_Texture2);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color3, m_Texture3);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, m_DepthStencilTexture);
	return rt;
}
