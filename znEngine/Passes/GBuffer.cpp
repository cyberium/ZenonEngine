#include "stdafx.h"

// General
#include "GBuffer.h"

// Additional
#include "Application.h"

CGBuffer::CGBuffer(std::shared_ptr<IScene> _worldScene)
	: m_Scene(_worldScene)
{
}

CGBuffer::~CGBuffer()
{
}

void CGBuffer::Load(uint32 width, uint32 height)
{
	IApplication& app = Application::Get();
	std::shared_ptr<IRenderDevice> renderDevice = app.GetRenderDevice();

	uint32 numSamples = 1;

	// Position (Color0) 
	ITexture::TextureFormat positionTextureFormat(
		ITexture::Components::RGBA,
		ITexture::Type::Float,
		numSamples,
		32, 32, 32, 32, 0, 0);
	std::shared_ptr<ITexture> positionTexture = renderDevice->CreateTexture2D(width, height, 1, positionTextureFormat);

	// Diffuse buffer (Color1)
	ITexture::TextureFormat duffuseTextureFormat(
		ITexture::Components::RGBA,
		ITexture::Type::UnsignedNormalized,
		numSamples,
		8, 8, 8, 8, 0, 0);
	std::shared_ptr<ITexture> duffuseTexture = renderDevice->CreateTexture2D(width, height, 1, duffuseTextureFormat);

	// Specular buffer (Color2)
	ITexture::TextureFormat specularTextureFormat(
		ITexture::Components::RGBA,
		ITexture::Type::UnsignedNormalized,
		numSamples,
		8, 8, 8, 8, 0, 0);
	std::shared_ptr<ITexture> specularTexture = renderDevice->CreateTexture2D(width, height, 1, specularTextureFormat);

	// Normal buffer (Color3)
	ITexture::TextureFormat normalTextureFormat(
		ITexture::Components::RGBA,
		ITexture::Type::UnsignedNormalized,
		numSamples,
		8, 8, 8, 8, 0, 0);
	std::shared_ptr<ITexture> normalTexture = renderDevice->CreateTexture2D(width, height, 1, normalTextureFormat);

	// Depth/stencil buffer
	ITexture::TextureFormat depthStencilTextureFormat(
		ITexture::Components::DepthStencil,
		ITexture::Type::UnsignedNormalized,
		numSamples,
		0, 0, 0, 0, 24, 8);
	std::shared_ptr<ITexture> depthStencilTexture = renderDevice->CreateTexture2D(width, height, 1, depthStencilTextureFormat);

	// Create a render target for the geometry pass.
	m_RenderTarget = renderDevice->CreateRenderTarget();

	// Use the render window's color attachment point for the "light accumulation" texture (no reason to have an additional buffer for this, that I'm aware of..)
	m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color0, positionTexture/*renderWindow->GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0)*/);
	m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color1, duffuseTexture);
	m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color2, specularTexture);
	m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color3, normalTexture);
	m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, depthStencilTexture /*renderWindow->GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil)*/);
}

void CGBuffer::Load2(const Viewport * _viewPort)
{
	IApplication& app = Application::Get();
	std::shared_ptr<IRenderDevice> renderDevice = app.GetRenderDevice();


	// Shaders that unite 4 textures
	std::shared_ptr<IShader> g_pVertexShader = _RenderDevice->CreateShader(IShader::ShaderType::VertexShader, "IDB_SHADER_DEFFERED_RENDERING", IShader::ShaderMacros(), "VS_main", "latest");
	std::vector<SCustomVertexElement> elements;
	elements.push_back({ 0, 0,  ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION, 0 });
	elements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD, 0 });
	elements.push_back({ 0, 20, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL, 0 });
	g_pVertexShader->LoadInputLayoutFromCustomElements(elements);


	std::shared_ptr<IShader> g_pDeferredLightingPixelShader = _RenderDevice->CreateShader(IShader::ShaderType::PixelShader, "IDB_SHADER_DEFFERED_RENDERING", IShader::ShaderMacros(), "PS_DeferredLighting", "latest");


	// Pipeline State for result texture
	IBlendState::BlendMode alphaBlending(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
	IBlendState::BlendMode additiveBlending(true, false, IBlendState::BlendFactor::One, IBlendState::BlendFactor::One);
	IBlendState::BlendMode disableBlending;
	IDepthStencilState::DepthMode enableDepthWrites(true, IDepthStencilState::DepthWrite::Enable);
	IDepthStencilState::DepthMode disableDepthWrites(false, IDepthStencilState::DepthWrite::Disable);


	std::shared_ptr<IRenderTarget> g_pDepthOnlyRenderTarget = renderDevice->CreateRenderTarget();
	//TODO:::: --------------- renderWINDOW g_pDepthOnlyRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, renderWindow->GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));


	// Pipeline for deferred lighting (stage 1 to determine lit pixels)
	std::shared_ptr<IPipelineState> g_pDeferredLightingPipeline1;
	{
		g_pDeferredLightingPipeline1 = renderDevice->CreatePipelineState();
		g_pDeferredLightingPipeline1->SetShader(IShader::ShaderType::VertexShader, g_pVertexShader);
		g_pDeferredLightingPipeline1->SetRenderTarget(g_pDepthOnlyRenderTarget);

		// Setup rasterizer state
		g_pDeferredLightingPipeline1->GetRasterizerState()->SetViewport(_viewPort);
		g_pDeferredLightingPipeline1->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
		g_pDeferredLightingPipeline1->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
		g_pDeferredLightingPipeline1->GetRasterizerState()->SetDepthClipEnabled(true);

		// Setup depth mode
		// Disable writing to the depth buffer.
		IDepthStencilState::DepthMode depthMode(true, IDepthStencilState::DepthWrite::Disable); // Disable depth writes.
		// Pass depth test if the light volume is behind scene geometry.
		depthMode.DepthFunction = IDepthStencilState::CompareFunction::Greater;
		//g_pDeferredLightingPipeline1->GetDepthStencilState().SetDepthMode(depthMode);

		// Setup stencil mode
		IDepthStencilState::StencilMode stencilMode(true); // Enable stencil operations
		IDepthStencilState::FaceOperation faceOperation;
		faceOperation.StencilDepthPass = IDepthStencilState::StencilOperation::DecrementClamp;
		stencilMode.StencilReference = 1;
		stencilMode.FrontFace = faceOperation;

		g_pDeferredLightingPipeline1->GetDepthStencilState()->SetStencilMode(stencilMode);
	}

	// Pipeline for deferred lighting (stage 2 to render lit pixels)
	std::shared_ptr<IPipelineState> g_pDeferredLightingPipeline2;
	{
		g_pDeferredLightingPipeline2 = renderDevice->CreatePipelineState();
		g_pDeferredLightingPipeline2->SetShader(IShader::ShaderType::VertexShader, g_pVertexShader);
		g_pDeferredLightingPipeline2->SetShader(IShader::ShaderType::PixelShader, g_pDeferredLightingPixelShader);
		//TODO:::: --------------- renderWINDOW g_pDeferredLightingPipeline2->SetRenderTarget(renderWindow->GetRenderTarget());

		// Setup rasterizer state.
		g_pDeferredLightingPipeline2->GetRasterizerState()->SetViewport(_viewPort);
		g_pDeferredLightingPipeline2->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Front);
		g_pDeferredLightingPipeline2->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
		g_pDeferredLightingPipeline2->GetRasterizerState()->SetDepthClipEnabled(false);

		// Perform additive blending if a pixel passes the depth/stencil tests.
		g_pDeferredLightingPipeline2->GetBlendState()->SetBlendMode(additiveBlending);

		// Setup depth mode
		// Disable depth writes
		IDepthStencilState::DepthMode depthMode(true, IDepthStencilState::DepthWrite::Disable); // Disable depth writes.
		depthMode.DepthFunction = IDepthStencilState::CompareFunction::GreaterOrEqual;
		//g_pDeferredLightingPipeline2->GetDepthStencilState().SetDepthMode(depthMode);

		// Setup stencil mode
		IDepthStencilState::StencilMode stencilMode(true);
		IDepthStencilState::FaceOperation faceOperation;
		// Render pixel if the depth function passes and the stencil was not un-marked in the previous pass.
		faceOperation.StencilFunction = IDepthStencilState::CompareFunction::Equal;
		stencilMode.StencilReference = 1;
		stencilMode.BackFace = faceOperation;

		g_pDeferredLightingPipeline2->GetDepthStencilState()->SetStencilMode(stencilMode);

	}

	// Pipeline for directional lights in deferred shader (only requires a single pass)
	std::shared_ptr<IPipelineState> g_pDirectionalLightsPipeline;
	{
		g_pDirectionalLightsPipeline = renderDevice->CreatePipelineState();
		g_pDirectionalLightsPipeline->SetShader(IShader::ShaderType::VertexShader, g_pVertexShader);
		g_pDirectionalLightsPipeline->SetShader(IShader::ShaderType::PixelShader, g_pDeferredLightingPixelShader);
		//TODO:::: --------------- renderWINDOW g_pDirectionalLightsPipeline->SetRenderTarget(renderWindow->GetRenderTarget());

		// Setup rasterizer state.
		g_pDirectionalLightsPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
		g_pDirectionalLightsPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
		g_pDirectionalLightsPipeline->GetRasterizerState()->SetViewport(_viewPort);

		// Perform additive blending if a pixel passes the depth/stencil tests.
		g_pDirectionalLightsPipeline->GetBlendState()->SetBlendMode(additiveBlending);

		// Setup depth mode
		IDepthStencilState::DepthMode depthMode(true, IDepthStencilState::DepthWrite::Disable); // Disable depth writes.
		// The full-screen quad that will be used to light pixels will be placed at the far clipping plane.
		// Only light pixels that are "in front" of the full screen quad (exclude sky box pixels)
		depthMode.DepthFunction = IDepthStencilState::CompareFunction::Greater;
		//g_pDirectionalLightsPipeline->GetDepthStencilState().SetDepthMode(depthMode);
	}


	m_Pass = std::make_shared<DeferredLightingPass>(
		_RenderDevice,
		m_Scene,
		g_pDeferredLightingPipeline1, g_pDeferredLightingPipeline2, g_pDirectionalLightsPipeline,
		m_RenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0), // position
		m_RenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color1), // diffuse
		m_RenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color2), // specular
		m_RenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color3), // normal
		m_RenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil)
	);
}

std::shared_ptr<IRenderTarget> CGBuffer::GetRenderTarget() const
{
	return m_RenderTarget;
}

std::shared_ptr<DeferredLightingPass> CGBuffer::GetPass() const
{
	return m_Pass;
}
