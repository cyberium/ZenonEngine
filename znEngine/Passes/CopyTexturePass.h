#pragma once

#include "AbstractPass.h"

class OW_ENGINE_API CopyTexturePass : public AbstractPass
{
public:
	CopyTexturePass(std::shared_ptr<Texture> destinationTexture, std::shared_ptr<Texture> sourceTexture);
	virtual ~CopyTexturePass();

	virtual void Render(RenderEventArgs& e);

private:
	std::shared_ptr<Texture> m_SourceTexture;
	std::shared_ptr<Texture> m_DestinationTexture;
};
