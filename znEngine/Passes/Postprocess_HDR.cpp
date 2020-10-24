#include "stdafx.h"

// General
#include "Postprocess_HDR.h"

CPassPostprocess_HDR::CPassPostprocess_HDR(IRenderDevice& RenderDevice, std::shared_ptr<IRenderTarget> HDRRenderTarget)
	: RenderPassPipelined(RenderDevice)
	, m_HDRRenderTarget(HDRRenderTarget)
{
	m_LightResultData = (SLightResult*)_aligned_malloc(sizeof(SLightResult), 16);
	m_LightResultConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(SLightResult());
}

CPassPostprocess_HDR::~CPassPostprocess_HDR()
{
	_aligned_free(m_LightResultData);
}



//
// IRenderPass
//

void CPassPostprocess_HDR::Render(RenderEventArgs& e)
{
	BindParamsForCurrentIteration(e);

	m_QuadGeometry->Render(e, GetPipeline().GetShaders().at(EShaderType::VertexShader).get());
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassPostprocess_HDR::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_QuadGeometry = GetRenderDevice().GetPrimitivesFactory().CreateQuad();

	auto samplesCnt = std::to_string(RenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0)->GetSamplesCount());

	auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Deffered_HDR.hlsl", "VS_ScreenQuad", { {"MULTISAMPLED", samplesCnt } });
	vertexShader->LoadInputLayoutFromReflector();

	auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Deffered_HDR.hlsl", "PS_ScreenQuad", { {"MULTISAMPLED", samplesCnt }});

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(additiveBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);
	

	auto& sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	GetPipeline().SetSampler(0, sampler);

	auto& samplerClamp = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	samplerClamp->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	samplerClamp->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	GetPipeline().SetSampler(1, samplerClamp);

	GetPipeline().SetTexture(0, m_HDRRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	//GetPipeline().SetTexture(1, m_HDRRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));

	return shared_from_this();
}



//
// Protected
//
void CPassPostprocess_HDR::BindParamsForCurrentIteration(const RenderEventArgs& e)
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
