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
				if (false == it.Node->IsEnabled())
					continue;

				DoRenderSceneNode(it.Node);
				DoRenderGeometry(it.Geometry, materialModel, it.GeometryDrawArgs);
			}
		}
	}
}

//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassDeffered_DoRenderScene::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/ModelVS.hlsl", "VS_PTN", { { "SKELETON_ANIMATION", "1" } });
	vertexShader->LoadInputLayoutFromReflector();

	/*std::vector<SCustomInputElement> customElements;
	customElements.push_back({ 0,  0, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION,     0 });
	customElements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD,     0 });
	customElements.push_back({ 0, 20, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL,       0 });
	customElements.push_back({ 0, 32, ECustomVertexElementType::FLOAT4, ECustomVertexElementUsage::BLENDWEIGHT,  0 });
	customElements.push_back({ 0, 48, ECustomVertexElementType::UINT4,  ECustomVertexElementUsage::BLENDINDICES, 0 });
	vertexShader->LoadInputLayoutFromCustomElements(customElements);*/

	// Per object
	m_PerObjectShaderParameter = &vertexShader->GetShaderParameterByName("PerObject");
	_ASSERT(m_PerObjectShaderParameter->IsValid());
	m_PerObjectShaderParameter->SetConstantBuffer(m_PerObjectConstantBuffer);

	// Bones
	m_ShaderBonesBufferParameter = &vertexShader->GetShaderParameterByName("Bones");
	//_ASSERT(m_ShaderBonesBufferParameter->IsValid());

	auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Model_Deffered.hlsl", "PS_main");

	// PIPELINES
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().SetRenderTarget(CreateGBuffer(RenderTarget));
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	return shared_from_this();
}



//
// IVisitor
//
void CPassDeffered_DoRenderScene::DoRenderSceneNode(const ISceneNode * SceneNode)
{
	m_PerObjectConstantBuffer->Set(PerObject(SceneNode->GetWorldTransfom()));
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

	if (m_ShaderBonesBufferParameter->IsValid())
		m_ShaderBonesBufferParameter->Bind();

	Material->Bind(shaders);
	Geometry->Render(vertexShader, GeometryDrawArgs);
	Material->Unbind(shaders);

	if (m_ShaderBonesBufferParameter->IsValid())
		m_ShaderBonesBufferParameter->Unbind();
}



//
// Private
//
std::shared_ptr<IRenderTarget> CPassDeffered_DoRenderScene::CreateGBuffer(std::shared_ptr<IRenderTarget> RenderTarget)
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
	m_Texture0 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(RenderTarget->GetViewport().GetWidth(), RenderTarget->GetViewport().GetHeight(), 1, colorTextureFormat);
	m_Texture1 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(RenderTarget->GetViewport().GetWidth(), RenderTarget->GetViewport().GetHeight(), 1, colorTextureFormat);
	m_Texture2 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(RenderTarget->GetViewport().GetWidth(), RenderTarget->GetViewport().GetHeight(), 1, normalTextureFormat);
	m_Texture3 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(RenderTarget->GetViewport().GetWidth(), RenderTarget->GetViewport().GetHeight(), 1, positionTextureFormat);

	// Depth/stencil buffer
	ITexture::TextureFormat depthStencilTextureFormat(
		ITexture::Components::DepthStencil,
		ITexture::Type::UnsignedNormalized,
		RenderTarget->GetSamplesCount(),
		0, 0, 0, 0, 24, 8);
	m_DepthStencilTexture = GetRenderDevice().GetObjectsFactory().CreateTexture2D(RenderTarget->GetViewport().GetWidth(), RenderTarget->GetViewport().GetHeight(), 1, depthStencilTextureFormat);

	auto rt = GetRenderDevice().GetObjectsFactory().CreateRenderTarget();
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color0, m_Texture0);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color1, m_Texture1);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color2, m_Texture2);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color3, m_Texture3);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, m_DepthStencilTexture);
	return rt;
}
