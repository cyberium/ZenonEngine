#include "stdafx.h"

// General
#include "GBuffer.h"

// Additional
#include "Application.h"

CGBuffer::CGBuffer(std::shared_ptr<Scene3D> _worldScene)
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
	std::shared_ptr<RenderWindow> renderWindow = app.GetRenderWindow();

	uint32 numSamples = 1;

	// Position (Color0) 
	Texture::TextureFormat positionTextureFormat(
		Texture::Components::RGBA,
		Texture::Type::Float,
		numSamples,
		32, 32, 32, 32, 0, 0);
	std::shared_ptr<Texture> positionTexture = renderDevice->CreateTexture2D(width, height, 1, positionTextureFormat);

	// Diffuse buffer (Color1)
	Texture::TextureFormat duffuseTextureFormat(
		Texture::Components::RGBA,
		Texture::Type::UnsignedNormalized,
		numSamples,
		8, 8, 8, 8, 0, 0);
	std::shared_ptr<Texture> duffuseTexture = renderDevice->CreateTexture2D(width, height, 1, duffuseTextureFormat);

	// Specular buffer (Color2)
	Texture::TextureFormat specularTextureFormat(
		Texture::Components::RGBA,
		Texture::Type::UnsignedNormalized,
		numSamples,
		8, 8, 8, 8, 0, 0);
	std::shared_ptr<Texture> specularTexture = renderDevice->CreateTexture2D(width, height, 1, specularTextureFormat);

	// Normal buffer (Color3)
	Texture::TextureFormat normalTextureFormat(
		Texture::Components::RGBA,
		Texture::Type::UnsignedNormalized,
		numSamples,
		8, 8, 8, 8, 0, 0);
	std::shared_ptr<Texture> normalTexture = renderDevice->CreateTexture2D(width, height, 1, normalTextureFormat);

	// Depth/stencil buffer
	Texture::TextureFormat depthStencilTextureFormat(
		Texture::Components::DepthStencil,
		Texture::Type::UnsignedNormalized,
		numSamples,
		0, 0, 0, 0, 24, 8);
	std::shared_ptr<Texture> depthStencilTexture = renderDevice->CreateTexture2D(width, height, 1, depthStencilTextureFormat);

	// Create a render target for the geometry pass.
	m_RenderTarget = renderDevice->CreateRenderTarget();

	// Use the render window's color attachment point for the "light accumulation" texture (no reason to have an additional buffer for this, that I'm aware of..)
	m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color0, positionTexture/*renderWindow->GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0)*/);
	m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color1, duffuseTexture);
	m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color2, specularTexture);
	m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color3, normalTexture);
	m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, depthStencilTexture /*renderWindow->GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil)*/);
}

void CGBuffer::Load2(const Viewport& _viewPort)
{
	IApplication& app = Application::Get();
	std::shared_ptr<IRenderDevice> renderDevice = app.GetRenderDevice();
	std::shared_ptr<RenderWindow> renderWindow = app.GetRenderWindow();


	// Shaders that unite 4 textures
	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(Shader::VertexShader, "shaders_D3D/DeferredRendering.hlsl", Shader::ShaderMacros(), "VS_main", "latest");
	std::vector<D3DVERTEXELEMENT9> elements;
	elements.push_back({ 0, 0,  D3DDECLTYPE_FLOAT3, 0, D3DDECLUSAGE_POSITION, 0 });
	elements.push_back({ 0, 12, D3DDECLTYPE_FLOAT2, 0, D3DDECLUSAGE_TEXCOORD, 0 });
	elements.push_back({ 0, 20, D3DDECLTYPE_FLOAT3, 0, D3DDECLUSAGE_NORMAL, 0 });
	g_pVertexShader->LoadInputLayoutFromD3DElement(elements);


	std::shared_ptr<Shader> g_pDeferredLightingPixelShader = _RenderDevice->CreateShader(Shader::PixelShader, "shaders_D3D/DeferredRendering.hlsl", Shader::ShaderMacros(), "PS_DeferredLighting", "latest");


	// Pipeline State for result texture
	BlendState::BlendMode alphaBlending(true, false, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendOperation::Add, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
	BlendState::BlendMode additiveBlending(true, false, BlendState::BlendFactor::One, BlendState::BlendFactor::One);
	BlendState::BlendMode disableBlending;
	DepthStencilState::DepthMode enableDepthWrites(true, DepthStencilState::DepthWrite::Enable);
	DepthStencilState::DepthMode disableDepthWrites(false, DepthStencilState::DepthWrite::Disable);


	std::shared_ptr<IRenderTarget> g_pDepthOnlyRenderTarget = renderDevice->CreateRenderTarget();
	g_pDepthOnlyRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, renderWindow->GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));


	// Pipeline for deferred lighting (stage 1 to determine lit pixels)
	std::shared_ptr<PipelineState> g_pDeferredLightingPipeline1;
	{
		g_pDeferredLightingPipeline1 = renderDevice->CreatePipelineState();
		g_pDeferredLightingPipeline1->SetShader(Shader::VertexShader, g_pVertexShader);
		g_pDeferredLightingPipeline1->SetRenderTarget(g_pDepthOnlyRenderTarget);

		// Setup rasterizer state
		g_pDeferredLightingPipeline1->GetRasterizerState().SetViewport(_viewPort);
		g_pDeferredLightingPipeline1->GetRasterizerState().SetCullMode(RasterizerState::CullMode::Back);
		g_pDeferredLightingPipeline1->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Solid);
		g_pDeferredLightingPipeline1->GetRasterizerState().SetDepthClipEnabled(true);

		// Setup depth mode
		// Disable writing to the depth buffer.
		DepthStencilState::DepthMode depthMode(true, DepthStencilState::DepthWrite::Disable); // Disable depth writes.
		// Pass depth test if the light volume is behind scene geometry.
		depthMode.DepthFunction = DepthStencilState::CompareFunction::Greater;
		//g_pDeferredLightingPipeline1->GetDepthStencilState().SetDepthMode(depthMode);

		// Setup stencil mode
		DepthStencilState::StencilMode stencilMode(true); // Enable stencil operations
		DepthStencilState::FaceOperation faceOperation;
		faceOperation.StencilDepthPass = DepthStencilState::StencilOperation::DecrementClamp;
		stencilMode.StencilReference = 1;
		stencilMode.FrontFace = faceOperation;

		g_pDeferredLightingPipeline1->GetDepthStencilState().SetStencilMode(stencilMode);
	}

	// Pipeline for deferred lighting (stage 2 to render lit pixels)
	std::shared_ptr<PipelineState> g_pDeferredLightingPipeline2;
	{
		g_pDeferredLightingPipeline2 = renderDevice->CreatePipelineState();
		g_pDeferredLightingPipeline2->SetShader(Shader::VertexShader, g_pVertexShader);
		g_pDeferredLightingPipeline2->SetShader(Shader::PixelShader, g_pDeferredLightingPixelShader);
		g_pDeferredLightingPipeline2->SetRenderTarget(renderWindow->GetRenderTarget());

		// Setup rasterizer state.
		g_pDeferredLightingPipeline2->GetRasterizerState().SetViewport(_viewPort);
		g_pDeferredLightingPipeline2->GetRasterizerState().SetCullMode(RasterizerState::CullMode::Front);
		g_pDeferredLightingPipeline2->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Solid);
		g_pDeferredLightingPipeline2->GetRasterizerState().SetDepthClipEnabled(false);

		// Perform additive blending if a pixel passes the depth/stencil tests.
		g_pDeferredLightingPipeline2->GetBlendState().SetBlendMode(additiveBlending);

		// Setup depth mode
		// Disable depth writes
		DepthStencilState::DepthMode depthMode(true, DepthStencilState::DepthWrite::Disable); // Disable depth writes.
		depthMode.DepthFunction = DepthStencilState::CompareFunction::GreaterOrEqual;
		//g_pDeferredLightingPipeline2->GetDepthStencilState().SetDepthMode(depthMode);

		// Setup stencil mode
		DepthStencilState::StencilMode stencilMode(true);
		DepthStencilState::FaceOperation faceOperation;
		// Render pixel if the depth function passes and the stencil was not un-marked in the previous pass.
		faceOperation.StencilFunction = DepthStencilState::CompareFunction::Equal;
		stencilMode.StencilReference = 1;
		stencilMode.BackFace = faceOperation;

		g_pDeferredLightingPipeline2->GetDepthStencilState().SetStencilMode(stencilMode);

	}

	// Pipeline for directional lights in deferred shader (only requires a single pass)
	std::shared_ptr<PipelineState> g_pDirectionalLightsPipeline;
	{
		g_pDirectionalLightsPipeline = renderDevice->CreatePipelineState();
		g_pDirectionalLightsPipeline->SetShader(Shader::VertexShader, g_pVertexShader);
		g_pDirectionalLightsPipeline->SetShader(Shader::PixelShader, g_pDeferredLightingPixelShader);
		g_pDirectionalLightsPipeline->SetRenderTarget(renderWindow->GetRenderTarget());

		// Setup rasterizer state.
		g_pDirectionalLightsPipeline->GetRasterizerState().SetCullMode(RasterizerState::CullMode::None);
		g_pDirectionalLightsPipeline->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Solid);
		g_pDirectionalLightsPipeline->GetRasterizerState().SetViewport(_viewPort);

		// Perform additive blending if a pixel passes the depth/stencil tests.
		g_pDirectionalLightsPipeline->GetBlendState().SetBlendMode(additiveBlending);

		// Setup depth mode
		DepthStencilState::DepthMode depthMode(true, DepthStencilState::DepthWrite::Disable); // Disable depth writes.
		// The full-screen quad that will be used to light pixels will be placed at the far clipping plane.
		// Only light pixels that are "in front" of the full screen quad (exclude sky box pixels)
		depthMode.DepthFunction = DepthStencilState::CompareFunction::Greater;
		//g_pDirectionalLightsPipeline->GetDepthStencilState().SetDepthMode(depthMode);
	}


	m_Pass = std::make_shared<DeferredLightingPass>(
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
