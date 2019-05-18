#pragma once

#include "AbstractPass.h"

class IRenderTarget;
class Textre;

/**
* A render pass that simply clears a render target.
*/
class GenerateMipMapPass : public AbstractPass
{
public:
	GenerateMipMapPass(std::shared_ptr<IRenderTarget> renderTarget);
	GenerateMipMapPass(std::shared_ptr<Texture> texture);
	virtual ~GenerateMipMapPass();

	virtual void Render(RenderEventArgs& e);

private:
	std::shared_ptr<IRenderTarget> m_RenderTarget;
	std::shared_ptr<Texture> m_Texture;
};
