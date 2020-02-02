#include "stdafx.h"

// General
#include "GenerateMipMapsPass.h"

GenerateMipMapPass::GenerateMipMapPass(IRenderDevice& RenderDevice, IRenderTarget* renderTarget)
	: RenderPass(RenderDevice)
	, m_RenderTarget(renderTarget)
{}

GenerateMipMapPass::GenerateMipMapPass(IRenderDevice& RenderDevice, ITexture* texture)
	: RenderPass(RenderDevice)
	, m_Texture(texture)
{}

GenerateMipMapPass::~GenerateMipMapPass()
{}

void GenerateMipMapPass::Render(RenderEventArgs& e)
{
	if (m_RenderTarget)
	{
		m_RenderTarget->GenerateMipMaps();
	}

	if (m_Texture)
	{
		m_Texture->GenerateMipMaps();
	}
}
