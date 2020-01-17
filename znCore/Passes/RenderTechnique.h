#pragma once

class ZN_API RenderTechnique 
	: public Object
{
public:
	RenderTechnique();
	virtual ~RenderTechnique();

	virtual uint32 AddPass(std::shared_ptr<IRenderPass> pass);
	virtual std::shared_ptr<IRenderPass> GetPass(uint32 ID) const;

	virtual void Render(RenderEventArgs& renderEventArgs);

	virtual void UpdateViewport(const Viewport * _viewport);

private:
	std::vector<std::shared_ptr<IRenderPass>>       m_Passes;
};