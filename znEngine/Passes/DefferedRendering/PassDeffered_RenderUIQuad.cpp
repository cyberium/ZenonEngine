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

struct __declspec(align(16)) SGPUDefferedLightVS
{
	SGPULightVS LightVS;
	// 96 bytes
	glm::mat4   LightViewMatrix;
	// 64 bytes
	glm::mat4   LightProjectionMatrix;
	// 64 bytes

};


CPassDeffered_RenderUIQuad::CPassDeffered_RenderUIQuad(IRenderDevice& RenderDevice, std::shared_ptr<CPassDeffered_DoRenderScene> DefferedRender, std::shared_ptr<CPassDeffered_ShadowMaps> DefferedRenderPrepareLights)
	: RenderPassPipelined(RenderDevice)
	, m_DefferedRender(DefferedRender)
	, m_Deffered_Lights(DefferedRenderPrepareLights)
{
	m_GPUDefferedLightVSBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(SGPUDefferedLightVS());
}

CPassDeffered_RenderUIQuad::~CPassDeffered_RenderUIQuad()
{
}



//
// IRenderPass
//
void CPassDeffered_RenderUIQuad::Render(RenderEventArgs& e)
{
	for (const auto& lightResult : m_Deffered_Lights->GetLightResult())
	{
		if (false == lightResult.IsEnabled)
			continue;

		FillLightParamsForCurrentIteration(e, lightResult);

		m_GPUDefferedLightVSParameter->Bind();
		m_ShadowMapTextureParameter->Bind();

		m_QuadGeometry->Render(GetPipeline().GetVertexShaderPtr());

		m_ShadowMapTextureParameter->Unbind();
		m_GPUDefferedLightVSParameter->Unbind();
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
	
	GetPipeline().SetTexture(0, m_DefferedRender->GetTexture0());
	GetPipeline().SetTexture(1, m_DefferedRender->GetTexture1());
	GetPipeline().SetTexture(2, m_DefferedRender->GetTexture2());
	GetPipeline().SetTexture(3, m_DefferedRender->GetTexture3());
	GetPipeline().SetTexture(4, m_DefferedRender->GetTextureDepthStencil());

	return shared_from_this();
}



//
// Protected
//
void CPassDeffered_RenderUIQuad::FillLightParamsForCurrentIteration(const RenderEventArgs& e, const CPassDeffered_ShadowMaps::SLightResult& LightResult)
{
	const ICameraComponent3D* camera = e.Camera;
	_ASSERT(camera != nullptr);

	auto lightOwner = LightResult.SceneNode;

	SGPUDefferedLightVS lightResult;

	// GPULightVS
	lightResult.LightVS.Light = LightResult.LightNode->GetGPULightStruct();
	lightResult.LightVS.LightPositionVS =                 camera->GetViewMatrix() * glm::vec4(lightOwner->GetPosition(), 1.0f);
	lightResult.LightVS.LightDirectionVS = glm::normalize(camera->GetViewMatrix() * glm::vec4(lightOwner->GetRotationEuler(), 0.0f));
	lightResult.LightVS.IsEnabled = LightResult.IsEnabled;
	lightResult.LightVS.IsCastShadow = LightResult.IsCastShadow;

	// GPUDefferedLightVS
	if (LightResult.IsCastShadow)
	{
		lightResult.LightViewMatrix = LightResult.LightNode->GetViewMatrix();
		lightResult.LightProjectionMatrix = LightResult.LightNode->GetProjectionMatrix();
	}

	// GPUDefferedLightVS
	m_GPUDefferedLightVSBuffer->Set(lightResult);
	m_GPUDefferedLightVSParameter->SetConstantBuffer(m_GPUDefferedLightVSBuffer);

	// ShadowMapTexture
	if (LightResult.IsCastShadow)
	{
		m_ShadowMapTextureParameter->SetTexture(LightResult.ShadowTexture);
	}
}
