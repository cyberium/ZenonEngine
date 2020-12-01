#include "stdafx.h"

// General
#include "PassDeffered_ProcessLights.h"

CPassDeffered_ProcessLights::CPassDeffered_ProcessLights(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneCreateTypelessListPass)
	: RenderPass(RenderDevice)
	, m_SceneCreateTypelessListPass(SceneCreateTypelessListPass)
{
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject());
	m_PerFrameConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerFrame());
}

CPassDeffered_ProcessLights::~CPassDeffered_ProcessLights()
{
}

void CPassDeffered_ProcessLights::CreateShadowPipeline()
{
	m_ShadowRenderTarget = CreateShadowRT();

	auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/ModelVS.hlsl", "VS_PTN", { { "SKELETON_ANIMATION", "1" } });
	vertexShader->LoadInputLayoutFromReflector();

	/*std::vector<SCustomInputElement> customElements;
	customElements.push_back({ 0,  0, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION,     0 });
	customElements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD,     0 });
	customElements.push_back({ 0, 20, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL,       0 });
	customElements.push_back({ 0, 32, ECustomVertexElementType::FLOAT4, ECustomVertexElementUsage::BLENDWEIGHT,  0 });
	customElements.push_back({ 0, 48, ECustomVertexElementType::UINT4,  ECustomVertexElementUsage::BLENDINDICES, 0 });
	vertexShader->LoadInputLayoutFromCustomElements(customElements);*/

	//auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Shadow.hlsl", "PS_Shadow");

	IBlendState::BlendMode disableBlending = IBlendState::BlendMode(false);
	IDepthStencilState::DepthMode enableDepthWrites = IDepthStencilState::DepthMode(true, IDepthStencilState::DepthWrite::Enable);

	// PIPELINES
	auto shadowPipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
	shadowPipeline->GetBlendState()->SetBlendMode(disableBlending);
	shadowPipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	//shadowPipeline->GetRasterizerState()->SetDepthBias(0.0f, 1.1f);
	shadowPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Front);
	shadowPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	shadowPipeline->SetRenderTarget(m_ShadowRenderTarget);
	shadowPipeline->SetShader(EShaderType::VertexShader, vertexShader);
	//shadowPipeline->SetShader(EShaderType::PixelShader, pixelShader);

	m_ShadowPipeline = shadowPipeline;

	m_PerObjectShaderParameter = vertexShader->GetShaderParameterByName("PerObject");
	_ASSERT(m_PerObjectShaderParameter->IsValid());
	m_PerObjectShaderParameter->SetConstantBuffer(m_PerObjectConstantBuffer);

	m_PerFrameShaderParameter = vertexShader->GetShaderParameterByName("PerFrame");
	_ASSERT(m_PerFrameShaderParameter->IsValid());
	m_PerFrameShaderParameter->SetConstantBuffer(m_PerFrameConstantBuffer);

	// Bones
	m_ShaderBonesBufferParameter = vertexShader->GetShaderParameterByName("Bones");
	//_ASSERT(m_ShaderBonesBufferParameter->IsValid());
}



const std::vector<CPassDeffered_ProcessLights::SLightResult>& CPassDeffered_ProcessLights::GetLightResult() const
{
	return m_LightResult;
}



//
// IRenderPass
//
void CPassDeffered_ProcessLights::PreRender(RenderEventArgs& e)
{
	RenderPass::PreRender(e);

	for (auto& it : m_LightResult)
	{
		it.IsLightEnabled = false;
	}
}

void CPassDeffered_ProcessLights::Render(RenderEventArgs& e)
{
	for (size_t i = 0; i < m_SceneCreateTypelessListPass->GetLightList().size(); i++)
	{
		const auto& lightIt = m_SceneCreateTypelessListPass->GetLightList().at(i);

		m_ShadowPipeline->Bind();
		{
			m_ShadowRenderTarget->Clear(ClearFlags::All, glm::vec4(0.0f), 1.0f);

			BindPerFrameParamsForCurrentIteration(lightIt.Light);

			for (const auto& geometryIt : m_SceneCreateTypelessListPass->GetGeometryList())
			{
				if (false == geometryIt.Node->IsEnabled())
					continue;

				if (false == geometryIt.Node->GetComponentT<IModelsComponent3D>()->IsCastShadows())
					continue;

				BindPerObjectParamsForCurrentIteration(geometryIt.Node);

				// Bones begin
				auto modelsComponent = geometryIt.Node->GetComponentT<IModelsComponent3D>();
				if (modelsComponent != nullptr)
				{
					if (m_ShaderBonesBufferParameter)
					{
						m_ShaderBonesBufferParameter->Set(modelsComponent->GetBonesBuffer());
						m_ShaderBonesBufferParameter->Bind();
					}
				}

				geometryIt.Geometry->Render(m_ShadowPipeline->GetShaders().at(EShaderType::VertexShader).get(), geometryIt.GeometryDrawArgs);

				// Bones end
				if (modelsComponent != nullptr)
				{
					if (m_ShaderBonesBufferParameter)
					{
						m_ShaderBonesBufferParameter->Unbind();
					}
				}
			}


			if (i < m_LightResult.size())
			{
				SLightResult& lightResult = m_LightResult.at(i);
				lightResult.SceneNode = lightIt.SceneNode;
				lightResult.LightNode = lightIt.Light;
				lightResult.IsLightEnabled = true;
				lightResult.IsCastShadow = true;
				lightResult.ShadowTexture->Copy(m_ShadowRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));
			}
			else
			{
				SLightResult lightResult;
				lightResult.SceneNode = lightIt.SceneNode;
				lightResult.LightNode = lightIt.Light;
				lightResult.IsLightEnabled = true;
				lightResult.IsCastShadow = true;
				lightResult.ShadowTexture = CreateShadowTextureDepthStencil();
				m_LightResult.push_back(lightResult);
			}
		}
		m_ShadowPipeline->UnBind();
	}
}

void CPassDeffered_ProcessLights::PostRender(RenderEventArgs& e)
{
	RenderPass::PostRender(e);
}



//
// Protected
//
std::shared_ptr<IRenderTarget> CPassDeffered_ProcessLights::CreateShadowRT()
{
	std::shared_ptr<IRenderTarget> rt = GetRenderDevice().GetObjectsFactory().CreateRenderTarget();
	//rt->AttachTexture(IRenderTarget::AttachmentPoint::Color0, CreateShadowTexture0());
	rt->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, CreateShadowTextureDepthStencil());
	rt->SetViewport(Viewport(cShadowTextureSize, cShadowTextureSize));
	return rt;
}

std::shared_ptr<ITexture> CPassDeffered_ProcessLights::CreateShadowTexture0() const
{
	ITexture::TextureFormat colorTextureFormat(
		ITexture::Components::R,
		ITexture::Type::Float,
		1,
		32, 0, 0, 0, 0, 0
	);
	return GetRenderDevice().GetObjectsFactory().CreateTexture2D(cShadowTextureSize, cShadowTextureSize, 1, colorTextureFormat);
}

std::shared_ptr<ITexture> CPassDeffered_ProcessLights::CreateShadowTextureDepthStencil() const
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

void CPassDeffered_ProcessLights::BindPerFrameParamsForCurrentIteration(const ILight3D * Light)
{
	const auto& viewport = m_ShadowPipeline->GetRenderTarget()->GetViewport();

	PerFrame perFrame(
		Light->GetViewMatrix(), 
		Light->GetProjectionMatrix(), 
		glm::vec2(viewport.GetWidth(), viewport.GetHeight()),
		glm::vec3(0.0f, 0.0f, 0.0f) // TODO FIXME
	);
	m_PerFrameConstantBuffer->Set(perFrame);
	m_PerFrameShaderParameter->Bind();
}

void CPassDeffered_ProcessLights::BindPerObjectParamsForCurrentIteration(const ISceneNode * SceneNode)
{
	m_PerObjectConstantBuffer->Set(PerObject(SceneNode->GetWorldTransfom()));
	m_PerObjectShaderParameter->Bind();
}
