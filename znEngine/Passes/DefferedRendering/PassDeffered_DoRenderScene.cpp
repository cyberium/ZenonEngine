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

void CPassDeffered_DoRenderScene::SetEnviorementTexture(std::shared_ptr<ITexture> Texture)
{
	GetPipeline().SetTexture(15, Texture);
}

//
// CPassDeffered_DoRenderScene
//
std::shared_ptr<IRenderTarget> CPassDeffered_DoRenderScene::GetGBufferRenderTarget() const
{
	return m_GBufferRenderTarget;
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
		if (false == it.Node->IsEnabled())
			continue;

		if (std::dynamic_pointer_cast<const MaterialModel>(it.Material) == nullptr)
			continue;

		if (it.Node->GetClass() == cSceneNode3D || it.Node->GetClass() == cSceneNodeRTSUnit || it.Node->GetClass() == cSceneNodeRTSBullet)
		{
			DoRenderSceneNode(it.Node);
			DoRenderGeometry(it.Geometry, it.Material, it.GeometryDrawArgs);
		}
	}
}

//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassDeffered_DoRenderScene::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	// Vertex shader
	{
		auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/ModelVS.hlsl", "VS_PTN", { { "SKELETON_ANIMATION", "1" } });
		vertexShader->LoadInputLayoutFromReflector();
		/*std::vector<SCustomInputElement> customElements;
		customElements.push_back({ 0,  0, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION,     0 });
		customElements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD,     0 });
		customElements.push_back({ 0, 20, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL,       0 });
		customElements.push_back({ 0, 32, ECustomVertexElementType::FLOAT4, ECustomVertexElementUsage::BLENDWEIGHT,  0 });
		customElements.push_back({ 0, 48, ECustomVertexElementType::UINT4,  ECustomVertexElementUsage::BLENDINDICES, 0 });
		vertexShader->LoadInputLayoutFromCustomElements(customElements);*/
		GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);

		// Per object
		m_PerObjectShaderParameter = vertexShader->GetShaderParameterByName("PerObject");
		_ASSERT(m_PerObjectShaderParameter);
		m_PerObjectShaderParameter->SetConstantBuffer(m_PerObjectConstantBuffer);

		// Bones
		m_ShaderBonesBufferParameter = vertexShader->GetShaderParameterByName("Bones");
		_ASSERT(m_ShaderBonesBufferParameter);
	}


	// Pixel shader
	{
		auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Deffered_MaterialModel.hlsl", "PS_main");
		GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);
	}


	// PIPELINES
	//GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Wireframe, IRasterizerState::FillMode::Wireframe);
	GetPipeline().GetRasterizerState()->SetDepthClipEnabled(false);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);

	m_GBufferRenderTarget = CreateGBuffer(RenderTarget);

	GetPipeline().SetRenderTarget(m_GBufferRenderTarget);
	
	return shared_from_this();
}



//
// IVisitor
//
void CPassDeffered_DoRenderScene::DoRenderSceneNode(const std::shared_ptr<const ISceneNode>& SceneNode)
{
	m_PerObjectConstantBuffer->Set(PerObject(SceneNode->GetWorldTransfom()));
	m_PerObjectShaderParameter->Bind();

	auto modelsComponent = SceneNode->GetComponentT<IModelComponent>();
	if (modelsComponent != nullptr)
		if (m_ShaderBonesBufferParameter)
			m_ShaderBonesBufferParameter->Set(modelsComponent->GetBonesSkinBuffer());
}

void CPassDeffered_DoRenderScene::DoRenderGeometry(const std::shared_ptr<const IGeometry>& Geometry, const std::shared_ptr<const IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs)
{
	if (m_ShaderBonesBufferParameter)
		m_ShaderBonesBufferParameter->Bind();

	Material->Bind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
	Geometry->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr(), GeometryDrawArgs);
	Material->Unbind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());

	if (m_ShaderBonesBufferParameter)
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
		ITexture::Type::Float,
		RenderTarget->GetSamplesCount(),
		16, 16, 16, 16, 0, 0
	);

	auto m_Texture0 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(RenderTarget->GetViewport().GetWidth(), RenderTarget->GetViewport().GetHeight(), 1, colorTextureFormat);
	auto m_Texture1 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(RenderTarget->GetViewport().GetWidth(), RenderTarget->GetViewport().GetHeight(), 1, colorTextureFormat);
	auto m_Texture2 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(RenderTarget->GetViewport().GetWidth(), RenderTarget->GetViewport().GetHeight(), 1, colorTextureFormat);
	auto m_Texture3 = GetRenderDevice().GetObjectsFactory().CreateTexture2D(RenderTarget->GetViewport().GetWidth(), RenderTarget->GetViewport().GetHeight(), 1, colorTextureFormat);

	// Depth/stencil buffer
	ITexture::TextureFormat depthStencilTextureFormat(
		ITexture::Components::DepthStencil,
		ITexture::Type::UnsignedNormalized,
		RenderTarget->GetSamplesCount(),
		0, 0, 0, 0, 24, 8);
	auto m_DepthStencilTexture = GetRenderDevice().GetObjectsFactory().CreateTexture2D(RenderTarget->GetViewport().GetWidth(), RenderTarget->GetViewport().GetHeight(), 1, depthStencilTextureFormat);

	auto rt = GetRenderDevice().GetObjectsFactory().CreateRenderTarget();
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color0, m_Texture0);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color1, m_Texture1);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color2, m_Texture2);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color3, m_Texture3);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, m_DepthStencilTexture);
	return rt;
}
