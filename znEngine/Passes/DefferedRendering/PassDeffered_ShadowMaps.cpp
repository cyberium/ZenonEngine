#include "stdafx.h"

// General
#include "PassDeffered_ShadowMaps.h"

CPassDeffered_ShadowMaps::CPassDeffered_ShadowMaps(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneCreateTypelessListPass)
	: RenderPass(RenderDevice)
	, m_SceneCreateTypelessListPass(SceneCreateTypelessListPass)
{
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject());
	m_PerFrameConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerFrame());
}

CPassDeffered_ShadowMaps::~CPassDeffered_ShadowMaps()
{
}

void CPassDeffered_ShadowMaps::CreateShadowPipeline()
{
	m_ShadowRenderTarget = CreateShadowRT();

	auto shadowPipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();

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

		m_PerObjectShaderParameter = vertexShader->GetShaderParameterByName("PerObject");
		_ASSERT(m_PerObjectShaderParameter);
		m_PerObjectShaderParameter->SetConstantBuffer(m_PerObjectConstantBuffer);

		m_PerFrameShaderParameter = vertexShader->GetShaderParameterByName("PerFrame");
		_ASSERT(m_PerFrameShaderParameter);
		m_PerFrameShaderParameter->SetConstantBuffer(m_PerFrameConstantBuffer);

		// Bones
		m_ShaderBonesBufferParameter = vertexShader->GetShaderParameterByName("Bones");
		//_ASSERT(m_ShaderBonesBufferParameter->IsValid());

		shadowPipeline->SetShader(vertexShader);
	}


	// Pixel shader
	{
		//auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Shadow.hlsl", "PS_Shadow");
		//shadowPipeline->SetShader(pixelShader);
	}

	IBlendState::BlendMode disableBlending = IBlendState::BlendMode(false);
	IDepthStencilState::DepthMode enableDepthWrites2 = IDepthStencilState::DepthMode(true, IDepthStencilState::DepthWrite::Enable);

	// PIPELINES
	//shadowPipeline->GetRasterizerState()->SetDepthClipEnabled(false);
	shadowPipeline->GetBlendState()->SetBlendMode(disableBlending);
	shadowPipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites2);
	//shadowPipeline->GetRasterizerState()->SetDepthBias(0.0f, 1.1f);
	shadowPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Front);
	shadowPipeline->SetRenderTarget(m_ShadowRenderTarget);
	
	m_ShadowPipeline = shadowPipeline;
}



//
// IRenderPass
//
void CPassDeffered_ShadowMaps::PreRender(RenderEventArgs& e)
{
	RenderPass::PreRender(e);

	m_CurrentShadowMapTexture = 0;
	m_ShadowMaps.clear();
}

void CPassDeffered_ShadowMaps::Render(RenderEventArgs& e)
{
	for (const auto& lightIt : m_SceneCreateTypelessListPass->GetLightList())
	{
		if (false == lightIt.Light->IsEnabled())
			continue;

		SShadowMap shadowMap;
		shadowMap.SceneNode = lightIt.SceneNode;
		shadowMap.LightNode = lightIt.Light;

		if (lightIt.Light->IsCastShadows())
		{
			m_ShadowPipeline->Bind();
			{
				m_ShadowRenderTarget->Clear();

				BindPerFrameParamsForCurrentIteration(lightIt.Light);

				RenderScene();

				auto shadowTexture = GetShadowMapTexture();
				shadowTexture->Copy(m_ShadowRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));
				shadowMap.ShadowTexture = shadowTexture;
			}
			m_ShadowPipeline->UnBind();
		}

		m_ShadowMaps.push_back(shadowMap);
	}
}

void CPassDeffered_ShadowMaps::PostRender(RenderEventArgs& e)
{
	RenderPass::PostRender(e);
}



//
// IPassDeffered_ShadowMaps
//
const ShadowMaps_t& CPassDeffered_ShadowMaps::GetShadowMaps() const
{
	return m_ShadowMaps;
}



//
// Protected
//
std::shared_ptr<IRenderTarget> CPassDeffered_ShadowMaps::CreateShadowRT()
{
	std::shared_ptr<IRenderTarget> rt = GetRenderDevice().GetObjectsFactory().CreateRenderTarget();
	//rt->AttachTexture(IRenderTarget::AttachmentPoint::Color0, CreateShadowTexture0());
	rt->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, CreateShadowTextureDepthStencil());
	rt->SetViewport(Viewport(cShadowTextureSize, cShadowTextureSize));
	return rt;
}

std::shared_ptr<ITexture> CPassDeffered_ShadowMaps::CreateShadowTexture0() const
{
	// Color buffer
	ITexture::TextureFormat colorTextureFormat(
		ITexture::Components::R,
		ITexture::Type::Float,
		1,
		32, 0, 0, 0, 0, 0
	);
	return GetRenderDevice().GetObjectsFactory().CreateTexture2D(cShadowTextureSize, cShadowTextureSize, 1, colorTextureFormat);
}

std::shared_ptr<ITexture> CPassDeffered_ShadowMaps::CreateShadowTextureDepthStencil() const
{
	// Depth/stencil buffer
	ITexture::TextureFormat depthStencilTextureFormat(
		ITexture::Components::DepthStencil,
		ITexture::Type::UnsignedNormalized,
		1,
		0, 0, 0, 0, 24, 8
	);
	return GetRenderDevice().GetObjectsFactory().CreateTexture2D(cShadowTextureSize, cShadowTextureSize, 1, depthStencilTextureFormat);
}

void CPassDeffered_ShadowMaps::RenderScene()
{
	for (const auto& geometryIt : m_SceneCreateTypelessListPass->GetGeometryList())
	{
		if (false == geometryIt.Node->IsEnabled())
			continue;

		auto modelsComponent = geometryIt.Node->GetComponentT<IModelComponent>();
		if (modelsComponent == nullptr)
			continue;

		if (false == modelsComponent->IsCastShadows())
			continue;

		BindPerObjectParamsForCurrentIteration(geometryIt.Node);

		// Bones begin
		if (m_ShaderBonesBufferParameter)
		{
			m_ShaderBonesBufferParameter->Set(modelsComponent->GetBonesSkinBuffer());
			m_ShaderBonesBufferParameter->Bind();
		}

		geometryIt.Geometry->Render(m_ShadowPipeline->GetVertexShaderPtr(), geometryIt.GeometryDrawArgs);

		// Bones end
		if (m_ShaderBonesBufferParameter)
		{
			m_ShaderBonesBufferParameter->Unbind();
		}
	}
}

void CPassDeffered_ShadowMaps::BindPerFrameParamsForCurrentIteration(const std::shared_ptr<const ILight>& Light)
{
	const auto& viewport = m_ShadowPipeline->GetRenderTarget()->GetViewport();

	PerFrame perFrame(
		Light->GetViewMatrix(), 
		Light->GetProjectionMatrix(), 
		viewport.GetSize()
	);
	m_PerFrameConstantBuffer->Set(perFrame);
	m_PerFrameShaderParameter->Bind();
}

void CPassDeffered_ShadowMaps::BindPerObjectParamsForCurrentIteration(const std::shared_ptr<const ISceneNode>& SceneNode)
{
	m_PerObjectConstantBuffer->Set(PerObject(SceneNode->GetWorldTransfom()));
	m_PerObjectShaderParameter->Bind();
}

std::shared_ptr<ITexture> CPassDeffered_ShadowMaps::GetShadowMapTexture()
{
	// Out of bounds. Add new texture.
	if (m_CurrentShadowMapTexture >= m_ShadowMapsTexturesCache.size())
	{
		auto newShadowMapTexture = CreateShadowTextureDepthStencil();
		m_ShadowMapsTexturesCache.push_back(newShadowMapTexture);
	}
	
	_ASSERT(m_CurrentShadowMapTexture < m_ShadowMapsTexturesCache.size());
	return m_ShadowMapsTexturesCache.at(m_CurrentShadowMapTexture++);
}
