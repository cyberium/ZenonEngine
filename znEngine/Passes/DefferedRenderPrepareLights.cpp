#include "stdafx.h"

// General
#include "DefferedRenderPrepareLights.h"

CDefferedRenderPrepareLights::CDefferedRenderPrepareLights(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<BuildRenderListPass> BuildRenderListPass)
	: RenderPassPipelined(RenderDevice)
	, m_BuildRenderListPass(BuildRenderListPass)
{
	m_PerObjectData = (PerObject3D*)_aligned_malloc(sizeof(PerObject3D), 16);
	m_PerObjectConstantBuffer = GetRenderDevice()->CreateConstantBuffer(PerObject3D());
}

CDefferedRenderPrepareLights::~CDefferedRenderPrepareLights()
{
	_aligned_free(m_PerObjectData);
	GetRenderDevice()->DestroyConstantBuffer(m_PerObjectConstantBuffer);
}

const std::vector<CDefferedRenderPrepareLights::SLightResult>& CDefferedRenderPrepareLights::GetLightResult() const
{
	return m_LightResult;
}



//
// IRenderPass
//
void CDefferedRenderPrepareLights::PreRender(RenderEventArgs& e)
{
	RenderPassPipelined::PreRender(e);

	for (auto& it : m_LightResult)
	{
		if (it.IsShadowEnable)
		{
			GetRenderDevice()->DestroyTexture(it.ShadowTexture);
			it.ShadowTexture = nullptr;
		}
	}
	m_LightResult.clear();
}

void CDefferedRenderPrepareLights::Render(RenderEventArgs& e)
{
	for (const auto& lightIt : m_BuildRenderListPass->GetLightList())
	{
		m_ShadowRenderTarget->Clear(ClearFlags::All, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

		BindPerFrameParamsForCurrentIteration(lightIt.Light);

		for (const auto& geometryIt : m_BuildRenderListPass->GetGeometryList())
		{
			BindPerObjectParamsForCurrentIteration(geometryIt.Node);
			geometryIt.Geometry->Render(&e, nullptr, GetPipeline()->GetShaders(), nullptr, geometryIt.GeometryPartParams);
		}

		SLightResult lightResult;
		lightResult.LightSceneNode = lightIt.Node;
		lightResult.LightComponent = lightIt.Light;
		lightResult.IsShadowEnable = true;
		lightResult.ShadowTexture = CreateShadowTexture();

		lightResult.ShadowTexture->Copy(m_ShadowRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
		//m_ShadowRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0)->Copy(lightResult.ShadowTexture);

		m_LightResult.push_back(lightResult);
	}
}

void CDefferedRenderPrepareLights::PostRender(RenderEventArgs& e)
{
	RenderPassPipelined::PostRender(e);
}



//
// IRenderPassPipelined
//
void CDefferedRenderPrepareLights::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	// Depth/stencil buffer
	ITexture::TextureFormat depthStencilTextureFormat(
		ITexture::Components::DepthStencil,
		ITexture::Type::UnsignedNormalized,
		1,
		0, 0, 0, 0, 24, 8);

	m_ShadowRenderTarget = GetRenderDevice()->CreateRenderTarget();
	m_ShadowRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color0, CreateShadowTexture());
	m_ShadowRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, GetRenderDevice()->CreateTexture2D(cShadowTextureSize, cShadowTextureSize, 1, depthStencilTextureFormat));

	v.SetWidth(cShadowTextureSize);
	v.SetHeight(cShadowTextureSize);

	std::shared_ptr<IShader> vertexShader = GetRenderDevice()->CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_SHADOW", IShader::ShaderMacros(), "VS_Shadow", "latest");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice()->CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_SHADOW", IShader::ShaderMacros(), "PS_Shadow", "latest");

	// PIPELINES
	std::shared_ptr<IPipelineState> shadowPipeline = GetRenderDevice()->CreatePipelineState();
	shadowPipeline->GetBlendState()->SetBlendMode(disableBlending);
	shadowPipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	shadowPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	shadowPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	shadowPipeline->SetRenderTarget(m_ShadowRenderTarget);
	shadowPipeline->GetRasterizerState()->SetViewport(&v);
	shadowPipeline->SetShader(EShaderType::VertexShader, vertexShader);
	shadowPipeline->SetShader(EShaderType::PixelShader, pixelShader);

	SetPipeline(shadowPipeline);
}

void CDefferedRenderPrepareLights::UpdateViewport(const Viewport * _viewport)
{
}



//
// Protected
//
std::shared_ptr<ITexture> CDefferedRenderPrepareLights::CreateShadowTexture() const
{
	ITexture::TextureFormat colorTextureFormat
	(
		ITexture::Components::R,
		ITexture::Type::Float,
		1,
		32, 0, 0, 0, 0, 0
	);
	return GetRenderDevice()->CreateTexture2D(cShadowTextureSize, cShadowTextureSize, 1, colorTextureFormat);
}

void CDefferedRenderPrepareLights::BindPerFrameParamsForCurrentIteration(const ILightComponent3D * LightComponent)
{
	m_PerFrameData->View = LightComponent->GetViewMatrix();
	m_PerFrameData->Projection = LightComponent->GetProjectionMatrix();
	m_PerFrameConstantBuffer->Set(*m_PerFrameData);

	const std::shared_ptr<IShaderParameter>& perFrameParam = GetPipeline()->GetShader(EShaderType::VertexShader)->GetShaderParameterByName("PerFrame");
	if (perFrameParam->IsValid() && m_PerFrameConstantBuffer != nullptr)
	{
		perFrameParam->SetConstantBuffer(m_PerFrameConstantBuffer.get());
		perFrameParam->Bind();
	}
	else
	{
		_ASSERT(false);
	}
}

void CDefferedRenderPrepareLights::BindPerObjectParamsForCurrentIteration(const ISceneNode * SceneNode)
{
	m_PerObjectData->Model = SceneNode->GetWorldTransfom();
	m_PerObjectConstantBuffer->Set(*m_PerObjectData);

	const std::shared_ptr<IShaderParameter>& perObjectParam = GetPipeline()->GetShader(EShaderType::VertexShader)->GetShaderParameterByName("PerObject");
	if (perObjectParam->IsValid() && m_PerObjectConstantBuffer != nullptr)
	{
		perObjectParam->SetConstantBuffer(m_PerObjectConstantBuffer.get());
		perObjectParam->Bind();
	}
	else
	{
		_ASSERT(false);
	}
}
