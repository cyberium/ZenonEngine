#include "stdafx.h"

// General
#include "PassDeffered_RenderUIQuad.h"


struct __declspec(align(16)) SGPULightVS
{
	SGPULight  Light;
	// 48 bytes
	glm::vec4  LightPositionVS;
	// 16 bytes
	glm::vec4  LightDirectionVS;
	// 16 bytes
	uint32      IsEnabled;
	uint32      IsCastShadow;
	glm::vec2   __padding;
	// 16 bytes
};


CPassDeffered_RenderUIQuad::CPassDeffered_RenderUIQuad(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneCreateTypelessListPass, std::shared_ptr<IRenderTarget> GBufferRenderTarget, std::shared_ptr<ITexture> MergedShadowTexture)
	: RenderPassPipelined(RenderDevice)
	, m_SceneCreateTypelessListPass(SceneCreateTypelessListPass)
	, m_GBufferRenderTarget(GBufferRenderTarget)
	, m_MergedShadowTexture(MergedShadowTexture)
{
	m_GPUDefferedLightVSBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(SGPULightVS());
}

CPassDeffered_RenderUIQuad::~CPassDeffered_RenderUIQuad()
{
}



//
// IRenderPass
//
void CPassDeffered_RenderUIQuad::Render(RenderEventArgs& e)
{

	m_ShadowMapTextureParameter->SetTexture(m_MergedShadowTexture);
	m_ShadowMapTextureParameter->Bind();

	for (const auto& lightIt : m_SceneCreateTypelessListPass->GetLightList())
	{
		FillLightParamsForCurrentIteration(e, lightIt);

		m_GPUDefferedLightVSParameter->Bind();
		m_QuadGeometry->Render(GetPipeline().GetVertexShaderPtr());
		m_GPUDefferedLightVSParameter->Unbind();
	}

	m_ShadowMapTextureParameter->Unbind();
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
		auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Deffered_UIQuad.hlsl", "VS_ScreenQuad", { {"MULTISAMPLED", samplesCnt.c_str() } });
		vertexShader->LoadInputLayoutFromReflector();
		GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	}


	{
		auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Deffered_UIQuad.hlsl", "PS_DeferredLighting", { {"MULTISAMPLED", samplesCnt.c_str() } });
		
		m_GPUDefferedLightVSParameter = pixelShader->GetShaderParameterByName("GPUDefferedLightVS");
		_ASSERT(m_GPUDefferedLightVSParameter);

		m_ShadowMapTextureParameter = pixelShader->GetShaderParameterByName("ShadowMapTexture");
		_ASSERT(m_ShadowMapTextureParameter);

		GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);
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

	SGPULightVS lightResult;

	// GPULightVS
	lightResult.Light = LightElement.Light->GetGPULightStruct();
	lightResult.LightPositionVS =                 camera->GetViewMatrix() * glm::vec4(LightElement.SceneNode->GetPosition(), 1.0f);
	lightResult.LightDirectionVS = glm::normalize(camera->GetViewMatrix() * glm::vec4(LightElement.SceneNode->GetRotationEuler(), 0.0f));
	lightResult.IsEnabled = LightElement.Light->IsEnabled();
	lightResult.IsCastShadow = LightElement.Light->IsCastShadows();

	// GPUDefferedLightVS
	m_GPUDefferedLightVSBuffer->Set(lightResult);
	m_GPUDefferedLightVSParameter->SetConstantBuffer(m_GPUDefferedLightVSBuffer);
}
