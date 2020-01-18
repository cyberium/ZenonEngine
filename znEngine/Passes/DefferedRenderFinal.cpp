#include "stdafx.h"

// General
#include "DefferedRenderFinal.h"

CDefferedRenderFinal::CDefferedRenderFinal(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<CDefferedRender> DefferedRender, std::shared_ptr<CDefferedRenderPrepareLights> DefferedRenderPrepareLights)
	: RenderPassPipelined(RenderDevice)
	, m_DefferedRender(DefferedRender)
	, m_DefferedRenderPrepareLights(DefferedRenderPrepareLights)
{
	m_ScreenToViewData = (SScreenToViewParams*)_aligned_malloc(sizeof(SScreenToViewParams), 16);
	m_ScreenToViewConstantBuffer = GetRenderDevice()->CreateConstantBuffer(SScreenToViewParams());

	m_LightResultData = (SLightResult*)_aligned_malloc(sizeof(SLightResult), 16);
	m_LightResultConstantBuffer = GetRenderDevice()->CreateConstantBuffer(SLightResult());
}

CDefferedRenderFinal::~CDefferedRenderFinal()
{
	_aligned_free(m_LightResultData);
	GetRenderDevice()->DestroyConstantBuffer(m_LightResultConstantBuffer);

	_aligned_free(m_ScreenToViewData);
	GetRenderDevice()->DestroyConstantBuffer(m_ScreenToViewConstantBuffer);
}



//
// IRenderPass
//
void CDefferedRenderFinal::PreRender(RenderEventArgs& e)
{
	RenderPassPipelined::PreRender(e);

	// Once per frame
	m_ScreenToViewData->InverseProjection = glm::inverse(e.Camera->GetProjectionMatrix());
	m_ScreenToViewData->ScreenDimensions = glm::vec2(e.PipelineState->GetRasterizerState()->GetViewports()[0]->GetWidth(), e.PipelineState->GetRasterizerState()->GetViewports()[0]->GetHeight());
	m_ScreenToViewConstantBuffer->Set(*m_ScreenToViewData);
	
	const std::shared_ptr<IShaderParameter>& screenToViewParams = GetPipeline()->GetShader(EShaderType::PixelShader)->GetShaderParameterByName("ScreenToViewParams");
	if (screenToViewParams->IsValid() && m_ScreenToViewConstantBuffer != nullptr)
	{
		screenToViewParams->SetConstantBuffer(m_ScreenToViewConstantBuffer.get());
		screenToViewParams->Bind();
	}
	// Once per frame
}

void CDefferedRenderFinal::Render(RenderEventArgs& e)
{
	for (const auto& lightResult : m_DefferedRenderPrepareLights->GetLightResult())
	{
		// Once per light
		BindLightParamsForCurrentIteration(e, lightResult);

		SGeometryPartParams GeometryPartParams;
		m_QuadMesh->GetGeometry()->Render(&e, nullptr, GetPipeline()->GetShaders(), nullptr, GeometryPartParams);
	}
}

void CDefferedRenderFinal::PostRender(RenderEventArgs& e)
{
	RenderPassPipelined::PostRender(e);
}



//
// IRenderPassPipelined
//
void CDefferedRenderFinal::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	std::shared_ptr<IShader> vertexShader = GetRenderDevice()->CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_DEFFERED", IShader::ShaderMacros(), "VS_ScreenQuad", "latest");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice()->CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_DEFFERED", IShader::ShaderMacros(), "PS_DeferredLighting", "latest");

	// PIPELINES
	std::shared_ptr<IPipelineState> defferedFinalPipeline = GetRenderDevice()->CreatePipelineState();
	defferedFinalPipeline->GetBlendState()->SetBlendMode(additiveBlending);
	defferedFinalPipeline->GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	defferedFinalPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	defferedFinalPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	defferedFinalPipeline->SetRenderTarget(RenderTarget);
	defferedFinalPipeline->GetRasterizerState()->SetViewport(Viewport);
	defferedFinalPipeline->SetShader(EShaderType::VertexShader, vertexShader);
	defferedFinalPipeline->SetShader(EShaderType::PixelShader, pixelShader);

	std::shared_ptr<ISamplerState> sampler = GetRenderDevice()->CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	defferedFinalPipeline->SetSampler(0, sampler);

	std::shared_ptr<ISamplerState> samplerClamp = GetRenderDevice()->CreateSamplerState();
	samplerClamp->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	samplerClamp->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	defferedFinalPipeline->SetSampler(1, samplerClamp);

	defferedFinalPipeline->SetTexture(0, m_DefferedRender->GetTexture0());
	defferedFinalPipeline->SetTexture(1, m_DefferedRender->GetTexture1());
	defferedFinalPipeline->SetTexture(2, m_DefferedRender->GetTexture2());
	defferedFinalPipeline->SetTexture(3, m_DefferedRender->GetTexture3());
	defferedFinalPipeline->SetTexture(4, m_DefferedRender->GetTextureDepthStencil());

	SetPipeline(defferedFinalPipeline);

	m_QuadMesh = GetRenderDevice()->GetPrimitiveCollection()->CreateQuad();
}



//
// Protected
//
void CDefferedRenderFinal::BindLightParamsForCurrentIteration(const RenderEventArgs& e, const CDefferedRenderPrepareLights::SLightResult& LightResult)
{
	const ICamera* camera = e.Camera;
	_ASSERT(camera != nullptr);

	SLightResult lightResult;

	lightResult.Light = LightResult.LightComponent->GetLightStruct();

	lightResult.Light.PositionVS = camera->GetViewMatrix() * glm::vec4(lightResult.Light.PositionWS.xyz(), 1.0f);
	lightResult.Light.DirectionVS = glm::normalize(camera->GetViewMatrix() * glm::vec4(lightResult.Light.DirectionWS.xyz(), 0.0f));

	lightResult.LightViewMatrix = LightResult.LightComponent->GetViewMatrix();
	lightResult.LightProjectionMatrix = LightResult.LightComponent->GetProjectionMatrix();

	lightResult.IsShadowEnabled = LightResult.IsShadowEnable;

	{
	    *m_LightResultData = lightResult;
		m_LightResultConstantBuffer->Set(*m_LightResultData);

		const std::shared_ptr<IShaderParameter>& lightParam = GetPipeline()->GetShader(EShaderType::PixelShader)->GetShaderParameterByName("LightResult");
		if (lightParam->IsValid() && m_LightResultConstantBuffer != nullptr)
		{
			lightParam->SetConstantBuffer(m_LightResultConstantBuffer.get());
			lightParam->Bind();
		}
		else
		{
			//_ASSERT(false);
		}
	}

	{
		const std::shared_ptr<IShaderParameter>& shadowTexture = GetPipeline()->GetShader(EShaderType::PixelShader)->GetShaderParameterByName("TextureShadow");
		if (shadowTexture->IsValid() && LightResult.IsShadowEnable && LightResult.ShadowTexture != nullptr)
		{
			shadowTexture->SetTexture(LightResult.ShadowTexture.get());
			shadowTexture->Bind();
		}
		else
		{
			//_ASSERT(false);
		}
	}
}
