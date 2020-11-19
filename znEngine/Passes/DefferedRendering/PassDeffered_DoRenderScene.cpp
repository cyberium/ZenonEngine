#include "stdafx.h"

// General
#include "PassDeffered_DoRenderScene.h"

// Additional
#include "Materials/MaterialModel.h"

CPassDeffered_DoRenderScene::CPassDeffered_DoRenderScene(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneCreateTypelessListPass)
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
		if (auto materialModel = dynamic_cast<const MaterialModel*>(it.Material))
		{
			if (it.Node->GetClass() == cSceneNode3D || it.Node->GetClass() == cSceneNodeRTSUnit || it.Node->GetClass() == cSceneNodeRTSBullet)
			{
				DoRenderSceneNode(it.Node);
				DoRenderGeometry(it.Geometry, materialModel, it.GeometryDrawArgs);
			}
		}
	}
}

//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassDeffered_DoRenderScene::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	__super::ConfigurePipeline(RenderTarget, Viewport);

	auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Model_Deffered.hlsl", "VS_PTN");
	vertexShader->LoadInputLayoutFromReflector();

	// Per object
	m_PerObjectShaderParameter = &vertexShader->GetShaderParameterByName("PerObject");
	_ASSERT(m_PerObjectShaderParameter->IsValid());
	m_PerObjectShaderParameter->SetConstantBuffer(m_PerObjectConstantBuffer);

	// Bones
	m_ShaderBonesBufferParameter = &vertexShader->GetShaderParameterByName("Bones");
	//_ASSERT(m_ShaderBonesBufferParameter->IsValid());

	auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Model_Deffered.hlsl", "PS_main");

	// PIPELINES
	GetPipeline().SetRenderTarget(CreateGBuffer(RenderTarget, Viewport));
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	return shared_from_this();
}



//
// IVisitor
//
void CPassDeffered_DoRenderScene::DoRenderSceneNode(const ISceneNode * SceneNode)
{
	PerObject perObject;
	perObject.Model = SceneNode->GetWorldTransfom();
	m_PerObjectConstantBuffer->Set(perObject);
	m_PerObjectShaderParameter->Bind();

	auto modelsComponent = SceneNode->GetComponentT<IModelsComponent3D>();
	if (modelsComponent != nullptr)
		if (m_ShaderBonesBufferParameter->IsValid())
			m_ShaderBonesBufferParameter->Set(modelsComponent->GetBonesBuffer());
}

void CPassDeffered_DoRenderScene::DoRenderGeometry(const IGeometry * Geometry, const IMaterial * Material, SGeometryDrawArgs GeometryDrawArgs)
{
	const auto& shaders = GetRenderEventArgs().PipelineState->GetShaders();
	const auto& vertexShader = shaders.at(EShaderType::VertexShader).get();

	Material->Bind(shaders);
	Geometry->Render(vertexShader, GeometryDrawArgs);
	Material->Unbind(shaders);
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
