#include <stdafx.h>

// General
#include "CopyTexturePass.h"

CopyTexturePass::CopyTexturePass(std::shared_ptr<ITexture> destinationTexture, std::shared_ptr<ITexture> sourceTexture)
	: m_DestinationTexture(destinationTexture)
	, m_SourceTexture(sourceTexture)
{}

CopyTexturePass::~CopyTexturePass()
{}

void CopyTexturePass::Render(RenderEventArgs& e)
{
	if (m_DestinationTexture)
	{
		m_DestinationTexture->Copy(m_SourceTexture);
	}
}
