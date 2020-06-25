#include "stdafx.h"

// General
#include "DefferedRenderPrepareLights.h"

CDefferedRenderPrepareLights::CDefferedRenderPrepareLights(IRenderDevice& RenderDevice, const std::shared_ptr<CSceneCreateTypelessListPass>& SceneCreateTypelessListPass)
	: RenderPass(RenderDevice)
	, m_SceneCreateTypelessListPass(SceneCreateTypelessListPass)
{
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject());
	m_PerFrameConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerFrame());
}

CDefferedRenderPrepareLights::~CDefferedRenderPrepareLights()
{
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
	RenderPass::PreRender(e);

	for (auto& it : m_LightResult)
	{
		it.IsEnabled = false;
	}
}

void CDefferedRenderPrepareLights::Render(RenderEventArgs& e)
{
	for (size_t i = 0; i < m_SceneCreateTypelessListPass->GetLightList().size(); i++)
	{
		const auto& lightIt = m_SceneCreateTypelessListPass->GetLightList().at(i);

		m_ShadowPipeline->Bind();
		{
			m_ShadowRenderTarget->Clear(ClearFlags::All, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

			BindPerFrameParamsForCurrentIteration(lightIt.Light);

			for (const auto& geometryIt : m_SceneCreateTypelessListPass->GetGeometryList())
			{
				BindPerObjectParamsForCurrentIteration(geometryIt.Node);

				geometryIt.Geometry->Render(e, m_ShadowPipeline->GetShaders().at(EShaderType::VertexShader).get(), geometryIt.GeometryDrawArgs);
			}

			if (i < m_LightResult.size())
			{
				SLightResult& lightResult = m_LightResult.at(i);
				lightResult.IsEnabled = true;
				lightResult.SceneNode = lightIt.SceneNode;
				lightResult.Light = lightIt.Light;
				lightResult.IsShadowEnable = true;
				lightResult.ShadowTexture->Copy(m_ShadowRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));
			}
			else
			{
				SLightResult lightResult;
				lightResult.IsEnabled = true;
				lightResult.SceneNode = lightIt.SceneNode;
				lightResult.Light = lightIt.Light;
				lightResult.IsShadowEnable = true;
				lightResult.ShadowTexture = CreateShadowTextureDepthStencil();

				const auto& ttPtr = m_ShadowRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil);

				//lightResult.ShadowTexture->Copy(ttPtr);
				m_LightResult.push_back(lightResult);
			}
		}
		m_ShadowPipeline->UnBind();
	}
}

void CDefferedRenderPrepareLights::PostRender(RenderEventArgs& e)
{
	RenderPass::PostRender(e);
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CDefferedRenderPrepareLights::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_ShadowViewport.SetWidth(cShadowTextureSize);
	m_ShadowViewport.SetHeight(cShadowTextureSize);

	m_ShadowRenderTarget = GetRenderDevice().GetObjectsFactory().CreateRenderTarget();
	//m_ShadowRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color0, CreateShadowTexture0());
	m_ShadowRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, CreateShadowTextureDepthStencil());
	m_ShadowRenderTarget->SetViewport(m_ShadowViewport);

	auto& vertexShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_SHADOW", "VS_Shadow");
	vertexShader->LoadInputLayoutFromReflector();

	//auto& pixelShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_SHADOW", "PS_Shadow", "latest");

	IBlendState::BlendMode disableBlending = IBlendState::BlendMode(false);
	IDepthStencilState::DepthMode enableDepthWrites = IDepthStencilState::DepthMode(true, IDepthStencilState::DepthWrite::Enable);

	// PIPELINES
	auto& shadowPipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
	shadowPipeline->GetBlendState()->SetBlendMode(disableBlending);
	shadowPipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	shadowPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	shadowPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	shadowPipeline->SetRenderTarget(m_ShadowRenderTarget);
	shadowPipeline->SetShader(EShaderType::VertexShader, vertexShader);
	//shadowPipeline->SetShader(EShaderType::PixelShader, pixelShader);

	m_ShadowPipeline = shadowPipeline;


	m_PerObjectShaderParameter = &vertexShader->GetShaderParameterByName("PerObject");
	_ASSERT(m_PerObjectShaderParameter->IsValid());
	m_PerObjectShaderParameter->SetConstantBuffer(m_PerObjectConstantBuffer);

	m_PerFrameShaderParameter = &vertexShader->GetShaderParameterByName("PerFrame");
	_ASSERT(m_PerFrameShaderParameter->IsValid());
	m_PerFrameShaderParameter->SetConstantBuffer(m_PerFrameConstantBuffer);

	return nullptr;
	//return SetPipeline(shadowPipeline);
}

void CDefferedRenderPrepareLights::UpdateViewport(const Viewport * _viewport)
{
}



//
// Protected
//
std::shared_ptr<ITexture> CDefferedRenderPrepareLights::CreateShadowTexture0() const
{
	ITexture::TextureFormat colorTextureFormat
	(
		ITexture::Components::R,
		ITexture::Type::Float,
		1,
		32, 0, 0, 0, 0, 0
	);
	return GetRenderDevice().GetObjectsFactory().CreateTexture2D(cShadowTextureSize, cShadowTextureSize, 1, colorTextureFormat);
}

std::shared_ptr<ITexture> CDefferedRenderPrepareLights::CreateShadowTextureDepthStencil() const
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

void CDefferedRenderPrepareLights::BindPerFrameParamsForCurrentIteration(const ILight3D * Light)
{
	const Viewport& viewport = GetRenderEventArgs()->PipelineState->GetRenderTarget()->GetViewport();

	PerFrame perFrame(
		Light->GetViewMatrix(), 
		Light->GetProjectionMatrix(), 
		glm::vec2(viewport.GetWidth(), viewport.GetHeight())
	);
	m_PerFrameConstantBuffer->Set(perFrame);

	m_PerFrameShaderParameter->Bind();
}

void CDefferedRenderPrepareLights::BindPerObjectParamsForCurrentIteration(const ISceneNode3D * SceneNode)
{
	PerObject perObject;
	perObject.Model = SceneNode->GetWorldTransfom();
	m_PerObjectConstantBuffer->Set(perObject);

	m_PerObjectShaderParameter->Bind();
}
