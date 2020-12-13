#include "stdafx.h"

// General
#include "PassDeffered_MergeShadowMaps.h"


struct __declspec(align(16)) SGPUShadowMap
{
	glm::mat4 LightViewMatrix;
	// 64 bytes
	glm::mat4 LightProjectionMatrix;
	// 64 bytes
};


CPassDeffered_MergeShadowMaps::CPassDeffered_MergeShadowMaps(IRenderDevice& RenderDevice, std::shared_ptr<IRenderTarget> GBufferRenderTarget, std::shared_ptr<CPassDeffered_ShadowMaps> DefferedRenderShadowMaps)
	: RenderPassPipelined(RenderDevice)
	, m_GBufferRenderTarget(GBufferRenderTarget)
	, m_DefferedRenderShadowMaps(DefferedRenderShadowMaps)
{
	m_ShadowMapBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(SGPUShadowMap());
}

CPassDeffered_MergeShadowMaps::~CPassDeffered_MergeShadowMaps()
{
}

std::shared_ptr<ITexture> CPassDeffered_MergeShadowMaps::GetMergedShadowMapTexture() const
{
	return m_MergedShadowMapTexture;
}



//
// IRenderPass
//
void CPassDeffered_MergeShadowMaps::Render(RenderEventArgs& e)
{
	GetPipeline().GetRenderTarget()->Clear();

	for (const auto& shadowMap : m_DefferedRenderShadowMaps->GetShadowMaps())
	{
		BindShadowMapToShader(e, shadowMap);

		m_ShadowMapParameter->Bind();
		m_ShadowMapTextureParameter->Bind();

		m_QuadGeometry->Render(GetPipeline().GetVertexShaderPtr());

		m_ShadowMapTextureParameter->Unbind();
		m_ShadowMapParameter->Unbind();
	}
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassDeffered_MergeShadowMaps::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	m_QuadGeometry = GetRenderDevice().GetPrimitivesFactory().CreateQuad();

	{
		auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Deffered_MergeShadows.hlsl", "VS_ScreenQuad");
		vertexShader->LoadInputLayoutFromReflector();
		GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	}

	{
		auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Deffered_MergeShadows.hlsl", "PS_Deferred_MergeShadows");
		
		m_ShadowMapParameter = pixelShader->GetShaderParameterByName("GPUDefferedMergeShadowMaps");
		_ASSERT(m_ShadowMapParameter);

		m_ShadowMapTextureParameter = pixelShader->GetShaderParameterByName("ShadowMapTexture");
		_ASSERT(m_ShadowMapTextureParameter);

		GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);
	}

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(additiveBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(disableDepthWrites);

	// Color buffer
	ITexture::TextureFormat colorTextureFormat(
		ITexture::Components::R,
		ITexture::Type::Float,
		1,
		16, 0, 0, 0, 0, 0
	);
	m_MergedShadowMapTexture = GetRenderDevice().GetObjectsFactory().CreateTexture2D(RenderTarget->GetViewport().GetWidth(), RenderTarget->GetViewport().GetHeight(), 1, colorTextureFormat);

	auto rt = GetRenderDevice().GetObjectsFactory().CreateRenderTarget();
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color0, m_MergedShadowMapTexture);
	GetPipeline().SetRenderTarget(rt);

	GetPipeline().SetTexture(4, m_GBufferRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));

	return shared_from_this();
}



//
// Protected
//
void CPassDeffered_MergeShadowMaps::BindShadowMapToShader(const RenderEventArgs& e, const CPassDeffered_ShadowMaps::SShadowMap& ShadowMap)
{
	const ICameraComponent3D* camera = e.Camera;
	_ASSERT(camera != nullptr);

	SGPUShadowMap shadowMap;

	// GPUDefferedLightVS
	shadowMap.LightViewMatrix = ShadowMap.LightNode->GetViewMatrix();
	shadowMap.LightProjectionMatrix = ShadowMap.LightNode->GetProjectionMatrix();

	// GPUDefferedLightVS
	m_ShadowMapBuffer->Set(shadowMap);
	m_ShadowMapParameter->SetConstantBuffer(m_ShadowMapBuffer);

	// ShadowMapTexture
	m_ShadowMapTextureParameter->SetTexture(ShadowMap.ShadowTexture);
}
