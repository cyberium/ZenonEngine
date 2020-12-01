#include "stdafx.h"

// General
#include "PassDeffered_RenderUIQuad.h"


struct __declspec(align(16)) SDefferedLightResult
{
	SLight     Light;
	glm::vec4  LightPositionVS;
	glm::vec4  LightDirectionVS;
	glm::mat4  LightViewMatrix;
	glm::mat4  LightProjectionMatrix;
};


CPassDeffered_RenderUIQuad::CPassDeffered_RenderUIQuad(IRenderDevice& RenderDevice, std::shared_ptr<CPassDeffered_DoRenderScene> DefferedRender, std::shared_ptr<CPassDeffered_ProcessLights> DefferedRenderPrepareLights)
	: RenderPassPipelined(RenderDevice)
	, m_DefferedRender(DefferedRender)
	, m_Deffered_Lights(DefferedRenderPrepareLights)
{
	m_LightResultConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(SDefferedLightResult());
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
		// Once per light
		BindLightParamsForCurrentIteration(e, lightResult);

		m_QuadGeometry->Render(GetPipeline().GetShaders().at(EShaderType::VertexShader).get());
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

	auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Deffered_UIQuad.hlsl", "VS_ScreenQuad", { {"MULTISAMPLED", samplesCnt.c_str() } });
	vertexShader->LoadInputLayoutFromReflector();

	auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Deffered_UIQuad.hlsl", "PS_DeferredLighting", { {"MULTISAMPLED", samplesCnt.c_str() }});

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(additiveBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	//GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Front);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);
	
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
void CPassDeffered_RenderUIQuad::BindLightParamsForCurrentIteration(const RenderEventArgs& e, const CPassDeffered_ProcessLights::SLightResult& LightResult)
{
	const ICameraComponent3D* camera = e.Camera;
	_ASSERT(camera != nullptr);

	const ISceneNode* lightOwner = LightResult.SceneNode;

	SDefferedLightResult lightResult;
	lightResult.Light = LightResult.LightNode->GetLightStruct();
	// From World space to camera space
	lightResult.LightPositionVS        = camera->GetViewMatrix() * glm::vec4(lightOwner->GetTranslation(),  1.0f);
	lightResult.LightDirectionVS       = glm::normalize(camera->GetViewMatrix() * glm::vec4(lightOwner->GetRotation(), 0.0f));
	lightResult.LightViewMatrix        = LightResult.LightNode->GetViewMatrix();
	lightResult.LightProjectionMatrix  = LightResult.LightNode->GetProjectionMatrix();



	{
		m_LightResultConstantBuffer->Set(lightResult);

		auto* lightParam = GetPipeline().GetPixelShaderPtr()->GetShaderParameterByName("LightResult");
		if (lightParam)
		{
			lightParam->SetConstantBuffer(m_LightResultConstantBuffer);
			lightParam->Bind();
		}
		else
		{
			//_ASSERT(false);
		}
	}



	{
		auto* shadowTexture = GetPipeline().GetPixelShaderPtr()->GetShaderParameterByName("TextureShadow");
		if (shadowTexture && LightResult.IsCastShadow && LightResult.ShadowTexture != nullptr)
		{
			shadowTexture->SetTexture(LightResult.ShadowTexture);
			shadowTexture->Bind();
		}
		else
		{
			//_ASSERT(false);
		}
	}
}
