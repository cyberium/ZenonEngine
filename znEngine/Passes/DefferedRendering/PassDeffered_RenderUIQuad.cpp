#include "stdafx.h"

// General
#include "PassDeffered_RenderUIQuad.h"





CPassDeffered_RenderUIQuad::CPassDeffered_RenderUIQuad(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneCreateTypelessListPass, std::shared_ptr<IRenderTarget> GBufferRenderTarget, std::shared_ptr<IPassDeffered_ShadowMaps> DefferedRenderShadowMaps)
	: RenderPassPipelined(RenderDevice)
	, m_SceneCreateTypelessListPass(SceneCreateTypelessListPass)
	, m_GBufferRenderTarget(GBufferRenderTarget)
	, m_ShadowMaps(DefferedRenderShadowMaps)
{
	m_GPULightVSDefferedBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(SGPULightVSDeffered());
}

CPassDeffered_RenderUIQuad::~CPassDeffered_RenderUIQuad()
{
}



//
// IRenderPass
//
void CPassDeffered_RenderUIQuad::Render(RenderEventArgs& e)
{
	for (const auto& map : m_ShadowMaps->GetShadowMaps())
	{
		if (map.LightNode->IsCastShadows())
		{
			m_ShadowMapTextureParameter->SetTexture(map.ShadowTexture);
			m_ShadowMapTextureParameter->Bind();
		}

		const auto& lightList = m_SceneCreateTypelessListPass->GetLightList();
		const auto& light = std::find_if(lightList.begin(), lightList.end(), [&map](const IRenderPassCreateTypelessList::SLightElement& LightElement) -> bool { return LightElement.Light == map.LightNode; });
		if (light == lightList.end())
			throw CException("Light not found!!!");

		FillLightParamsForCurrentIteration(e, *light);

		m_GPUDefferedLightVSParameter->SetConstantBuffer(m_GPULightVSDefferedBuffer);
		m_GPUDefferedLightVSParameter->Bind();
		{
			m_QuadGeometry->Render(GetPipeline().GetVertexShaderPtr());
		}
		m_GPUDefferedLightVSParameter->Unbind();

		if (map.LightNode->IsCastShadows())
		{
			m_ShadowMapTextureParameter->Unbind();
		}
	}
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassDeffered_RenderUIQuad::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	m_QuadGeometry = GetRenderDevice().GetPrimitivesFactory().CreateQuad();

	auto samplesCnt = std::to_string(RenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0)->GetSamplesCount());

	{
		auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Deffered/Deffered_UIQuad.hlsl", "VS_ScreenQuad", { {"MULTISAMPLED", samplesCnt.c_str() } });
		vertexShader->LoadInputLayoutFromReflector();
		GetPipeline().SetShader(vertexShader);
	}


	{
		auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Deffered/Deffered_UIQuad.hlsl", "PS_DeferredLighting", { {"MULTISAMPLED", samplesCnt.c_str() } });
		
		m_GPUDefferedLightVSParameter = pixelShader->GetShaderParameterByName("GPUDefferedLightVS");
		_ASSERT(m_GPUDefferedLightVSParameter);

		m_ShadowMapTextureParameter = pixelShader->GetShaderParameterByName("ShadowMapTexture");
		_ASSERT(m_ShadowMapTextureParameter);

		GetPipeline().SetShader(pixelShader);
	}


	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(additiveBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	
	GetPipeline().SetTexture(0, m_GBufferRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	GetPipeline().SetTexture(1, m_GBufferRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color1));
	GetPipeline().SetTexture(2, m_GBufferRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color2));
	GetPipeline().SetTexture(3, m_GBufferRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color3));
	GetPipeline().SetTexture(4, m_GBufferRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));

	return shared_from_this();
}



//
// Protected
//
void CPassDeffered_RenderUIQuad::FillLightParamsForCurrentIteration(const RenderEventArgs& e, const IRenderPassCreateTypelessList::SLightElement& LightElement)
{
	const ICameraComponent3D* camera = e.Camera;
	_ASSERT(camera != nullptr);

	SGPULightVSDeffered lightResult;

	// GPULightVS
	lightResult.LightVS.Light = LightElement.Light->GetGPULightStruct();
	lightResult.LightVS.LightPositionVS =                 camera->GetViewMatrix() * glm::vec4(LightElement.SceneNode->GetPosition(), 1.0f);
	lightResult.LightVS.LightDirectionVS = glm::normalize(camera->GetViewMatrix() * glm::vec4(LightElement.SceneNode->GetLocalRotationDirection(), 0.0f));
	lightResult.LightVS.IsEnabled = LightElement.Light->IsEnabled();
	lightResult.LightVS.IsCastShadow = LightElement.Light->IsCastShadows();
	
	// GPULightVSDeffered
	if (LightElement.Light->IsCastShadows())
	{
		lightResult.LightViewMatrix = LightElement.Light->GetViewMatrix();
		lightResult.LightProjectionMatrix = LightElement.Light->GetProjectionMatrix();
	}

	m_GPULightVSDefferedBuffer->Set(lightResult);
}
