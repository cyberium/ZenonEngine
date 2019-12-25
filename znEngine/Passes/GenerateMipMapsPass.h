#pragma once

class ZN_API GenerateMipMapPass : public AbstractPass
{
public:
	GenerateMipMapPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IRenderTarget> renderTarget);
	GenerateMipMapPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<ITexture> texture);
	virtual ~GenerateMipMapPass();

	virtual void Render(RenderEventArgs& e);

private:
	std::shared_ptr<IRenderTarget> m_RenderTarget;
	std::shared_ptr<ITexture> m_Texture;
};
