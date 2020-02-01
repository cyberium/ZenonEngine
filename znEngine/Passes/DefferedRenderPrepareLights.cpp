#include "stdafx.h"

// General
#include "DefferedRenderPrepareLights.h"

CDefferedRenderPrepareLights::CDefferedRenderPrepareLights(IRenderDevice* RenderDevice, std::shared_ptr<BuildRenderListPass> BuildRenderListPass)
	: RenderPass(RenderDevice)
	, m_BuildRenderListPass(BuildRenderListPass)
{
	m_PerObjectData = (PerObject3D*)_aligned_malloc(sizeof(PerObject3D), 16);
	m_PerObjectConstantBuffer = GetRenderDevice()->CreateConstantBuffer(PerObject3D());

	m_PerFrameConstantBuffer = GetRenderDevice()->CreateConstantBuffer(PerFrame());
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
	RenderPass::PreRender(e);

	for (auto& it : m_LightResult)
	{
		it.IsEnabled = false;
	}
}

void CDefferedRenderPrepareLights::Render(RenderEventArgs& e)
{
	for (size_t i = 0; i < m_BuildRenderListPass->GetLightList().size(); i++)
	{


		const auto& lightIt = m_BuildRenderListPass->GetLightList().at(i);

		m_ShadowPipeline->Bind();
		{
			m_ShadowRenderTarget->Clear(ClearFlags::All, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

			BindPerFrameParamsForCurrentIteration(lightIt.Light);

			for (const auto& geometryIt : m_BuildRenderListPass->GetGeometryList())
			{
				BindPerObjectParamsForCurrentIteration(geometryIt.Node);

				geometryIt.Geometry->Render(&e, nullptr, m_ShadowPipeline->GetShaders(), nullptr, geometryIt.GeometryPartParams);
			}

			if (i < m_LightResult.size())
			{
				SLightResult& lightResult = m_LightResult.at(i);
				lightResult.IsEnabled = true;
				lightResult.LightSceneNode = lightIt.Node;
				lightResult.LightComponent = lightIt.Light;
				lightResult.IsShadowEnable = true;
				lightResult.ShadowTexture->Copy(m_ShadowRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));
			}
			else
			{
				SLightResult lightResult;
				lightResult.IsEnabled = true;
				lightResult.LightSceneNode = lightIt.Node;
				lightResult.LightComponent = lightIt.Light;
				lightResult.IsShadowEnable = true;
				lightResult.ShadowTexture = CreateShadowTextureDepthStencil();
				lightResult.ShadowTexture->Copy(m_ShadowRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));
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
std::shared_ptr<IRenderPassPipelined> CDefferedRenderPrepareLights::CreatePipeline(IRenderTarget* RenderTarget, const Viewport * Viewport)
{
	m_ShadowRenderTarget = GetRenderDevice()->CreateRenderTarget();
	//m_ShadowRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color0, CreateShadowTexture0());
	m_ShadowRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, CreateShadowTextureDepthStencil());

	m_ShadowViewport.SetWidth(cShadowTextureSize);
	m_ShadowViewport.SetHeight(cShadowTextureSize);

	IShader* vertexShader = GetRenderDevice()->CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_SHADOW", IShader::ShaderMacros(), "VS_Shadow", "latest");
	vertexShader->LoadInputLayoutFromReflector();

	//IShader* pixelShader = GetRenderDevice()->CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_SHADOW", IShader::ShaderMacros(), "PS_Shadow", "latest");

	IBlendState::BlendMode disableBlending = IBlendState::BlendMode(false);
	IDepthStencilState::DepthMode enableDepthWrites = IDepthStencilState::DepthMode(true, IDepthStencilState::DepthWrite::Enable);

	// PIPELINES
	IPipelineState* shadowPipeline = GetRenderDevice()->CreatePipelineState();
	shadowPipeline->GetBlendState().SetBlendMode(disableBlending);
	shadowPipeline->GetDepthStencilState().SetDepthMode(enableDepthWrites);
	shadowPipeline->GetRasterizerState().SetCullMode(IRasterizerState::CullMode::None);
	shadowPipeline->GetRasterizerState().SetFillMode(IRasterizerState::FillMode::Solid);
	shadowPipeline->SetRenderTarget(m_ShadowRenderTarget);
	shadowPipeline->GetRasterizerState().SetViewport(&m_ShadowViewport);
	shadowPipeline->SetShader(EShaderType::VertexShader, vertexShader);
	//shadowPipeline->SetShader(EShaderType::PixelShader, pixelShader);

	m_ShadowPipeline = shadowPipeline;
	return nullptr;
	//return SetPipeline(shadowPipeline);
}

void CDefferedRenderPrepareLights::UpdateViewport(const Viewport * _viewport)
{
}



//
// Protected
//
ITexture* CDefferedRenderPrepareLights::CreateShadowTexture0() const
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

ITexture* CDefferedRenderPrepareLights::CreateShadowTextureDepthStencil() const
{
	// Depth/stencil buffer
	ITexture::TextureFormat depthStencilTextureFormat(
		ITexture::Components::DepthStencil,
		ITexture::Type::UnsignedNormalized,
		1,
		0, 0, 0, 0, 24, 8
	);
	return GetRenderDevice()->CreateTexture2D(cShadowTextureSize, cShadowTextureSize, 1, depthStencilTextureFormat);
}

void CDefferedRenderPrepareLights::DoRenderToOneContext()
{
}

void CDefferedRenderPrepareLights::BindPerFrameParamsForCurrentIteration(const ILightComponent3D * LightComponent)
{
	PerFrame perFrame;
	perFrame.View = LightComponent->GetViewMatrix();
	perFrame.Projection = LightComponent->GetProjectionMatrix();
	SetPerFrameData(perFrame);

	BindPerFrameDataToVertexShader(m_ShadowPipeline->GetShader(EShaderType::VertexShader));
}

void CDefferedRenderPrepareLights::BindPerObjectParamsForCurrentIteration(const ISceneNode3D * SceneNode)
{
	m_PerObjectData->Model = SceneNode->GetWorldTransfom();
	m_PerObjectConstantBuffer->Set(*m_PerObjectData);

	IShaderParameter* perObjectParam = m_ShadowPipeline->GetShader(EShaderType::VertexShader)->GetShaderParameterByName("PerObject");
	if (perObjectParam->IsValid() && m_PerObjectConstantBuffer != nullptr)
	{
		perObjectParam->SetConstantBuffer(m_PerObjectConstantBuffer);
		perObjectParam->Bind();
	}
	else
	{
		_ASSERT(false);
	}
}



//
// Protected
//
void CDefferedRenderPrepareLights::SetPerFrameData(const PerFrame& PerFrame)
{
	m_PerFrameConstantBuffer->Set(PerFrame);
}

void CDefferedRenderPrepareLights::BindPerFrameDataToVertexShader(const IShader * VertexShader) const
{
	IShaderParameter* perFrameParam = VertexShader->GetShaderParameterByName("PerFrame");
	if (perFrameParam->IsValid() && m_PerFrameConstantBuffer != nullptr)
	{
		perFrameParam->SetConstantBuffer(m_PerFrameConstantBuffer);
		perFrameParam->Bind();
	}
}
