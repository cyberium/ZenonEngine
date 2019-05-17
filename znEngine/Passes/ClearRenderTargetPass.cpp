#include <stdafx.h>

// General
#include "ClearRenderTargetPass.h"

ClearRenderTargetPass::ClearRenderTargetPass(std::shared_ptr<IRenderTarget> renderTarget, ClearFlags clearFlags, const glm::vec4& color, float depth, uint8_t stencil)
	: m_RenderTarget(renderTarget)
	, m_ClearFlags(clearFlags)
	, m_ClearColor(color)
	, m_ClearDepth(depth)
	, m_ClearStencil(stencil)
{}

ClearRenderTargetPass::ClearRenderTargetPass(std::shared_ptr<Texture> texture, ClearFlags clearFlags, const glm::vec4& color, float depth, uint8_t stencil)
	: m_Texture(texture)
	, m_ClearFlags(clearFlags)
	, m_ClearColor(color)
	, m_ClearDepth(depth)
	, m_ClearStencil(stencil)
{}

ClearRenderTargetPass::~ClearRenderTargetPass()
{}

void ClearRenderTargetPass::Render(Render3DEventArgs& e)
{
	if (m_RenderTarget)
	{
		m_RenderTarget->Clear(m_ClearFlags, m_ClearColor, m_ClearDepth, m_ClearStencil);
	}
	if (m_Texture)
	{
		m_Texture->Clear(m_ClearFlags, m_ClearColor, m_ClearDepth, m_ClearStencil);
	}
}
