#pragma once

#include "AbstractPass.h"

class OW_ENGINE_API CopyTexturePass : public AbstractPass
{
public:
	CopyTexturePass(std::shared_ptr<ITexture> destinationTexture, std::shared_ptr<ITexture> sourceTexture);
	virtual ~CopyTexturePass();

	virtual void Render(RenderEventArgs& e);

private:
	std::shared_ptr<ITexture> m_SourceTexture;
	std::shared_ptr<ITexture> m_DestinationTexture;
};
