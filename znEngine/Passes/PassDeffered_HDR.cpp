#include "stdafx.h"

// General
#include "PassDeffered_HDR.h"

CPassDeffered_HDR::CPassDeffered_HDR(IRenderDevice& RenderDevice, std::shared_ptr<IRenderTarget> HDRRenderTarget)
	: RenderPassPipelined(RenderDevice)
	, m_HDRRenderTarget(HDRRenderTarget)
{
	m_LightResultData = (SLightResult*)_aligned_malloc(sizeof(SLightResult), 16);
	m_LightResultConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(SLightResult());
}

CPassDeffered_HDR::~CPassDeffered_HDR()
{
	_aligned_free(m_LightResultData);
}



//
// IRenderPass
//

void CPassDeffered_HDR::Render(RenderEventArgs& e)
{
	BindParamsForCurrentIteration(e);

	m_QuadGeometry->Render(e, GetPipeline().GetShaders().at(EShaderType::VertexShader).get());
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassDeffered_HDR::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_QuadGeometry = GetRenderDevice().GetPrimitivesFactory().CreateQuad();

	auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Deffered_HDR.hlsl", "VS_ScreenQuad", { {"MULTISAMPLED", "1" } });
	vertexShader->LoadInputLayoutFromReflector();

	auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Deffered_HDR.hlsl", "PS_ScreenQuad", { {"MULTISAMPLED", "1" }});

	// PIPELINES
	auto defferedFinalPipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
	defferedFinalPipeline->GetBlendState()->SetBlendMode(additiveBlending);
	defferedFinalPipeline->GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	defferedFinalPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	defferedFinalPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	defferedFinalPipeline->SetRenderTarget(RenderTarget);
	defferedFinalPipeline->SetShader(EShaderType::VertexShader, vertexShader);
	defferedFinalPipeline->SetShader(EShaderType::PixelShader, pixelShader);
	

	auto& sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	defferedFinalPipeline->SetSampler(0, sampler);

	auto& samplerClamp = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	samplerClamp->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	samplerClamp->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	defferedFinalPipeline->SetSampler(1, samplerClamp);

	defferedFinalPipeline->SetTexture(0, m_HDRRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	//defferedFinalPipeline->SetTexture(1, m_HDRRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));

	return SetPipeline(defferedFinalPipeline);
}



//
// Protected
//
void CPassDeffered_HDR::BindParamsForCurrentIteration(const RenderEventArgs& e)
{
	/*
	const ICameraComponent3D* camera = e.Camera;
	_ASSERT(camera != nullptr);

	SLightResult lightResult;
	lightResult.Light = LightResult.Light->GetLightStruct();
	lightResult.Light.PositionVS = camera->GetViewMatrix() * glm::vec4(lightResult.Light.PositionWS.xyz(), 1.0f);
	lightResult.Light.DirectionVS = glm::normalize(camera->GetViewMatrix() * glm::vec4(lightResult.Light.DirectionWS.xyz(), 0.0f));
	lightResult.LightViewMatrix = LightResult.Light->GetViewMatrix();
	lightResult.LightProjectionMatrix = LightResult.Light->GetProjectionMatrix();
	lightResult.IsShadowEnabled = LightResult.IsShadowEnable;

	{
	    *m_LightResultData = lightResult;
		m_LightResultConstantBuffer->Set(*m_LightResultData);

		auto& lightParam = GetPipeline().GetShader(EShaderType::PixelShader)->GetShaderParameterByName("LightResult");
		if (lightParam.IsValid() && m_LightResultConstantBuffer != nullptr)
		{
			lightParam.SetConstantBuffer(m_LightResultConstantBuffer);
			lightParam.Bind();
		}
		else
		{
			//_ASSERT(false);
		}
	}

	{
		auto& shadowTexture = GetPipeline().GetShader(EShaderType::PixelShader)->GetShaderParameterByName("TextureShadow");
		if (shadowTexture.IsValid() && LightResult.IsShadowEnable && LightResult.ShadowTexture != nullptr)
		{
			shadowTexture.SetTexture(LightResult.ShadowTexture);
			shadowTexture.Bind();
		}
		else
		{
			//_ASSERT(false);
		}
	}
	*/
}
