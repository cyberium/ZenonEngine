#include <stdafx.h>

// General
#include "GenerateMipMapsPass.h"

GenerateMipMapPass::GenerateMipMapPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IRenderTarget> renderTarget)
	: AbstractPass(RenderDevice)
	, m_RenderTarget(renderTarget)
{}

GenerateMipMapPass::GenerateMipMapPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<ITexture> texture)
	: AbstractPass(RenderDevice)
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
