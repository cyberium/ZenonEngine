#pragma once

class ZN_API RenderTechnique
	: public IRenderTechnique
	, public Object
{
public:
	RenderTechnique();
	virtual ~RenderTechnique();

	// IRenderTechnique
	virtual uint32 AddPass(std::shared_ptr<IRenderPass> pass);
	virtual IRenderPass* GetPass(uint32 ID) const;

	virtual void Render(RenderEventArgs& renderEventArgs);

private:
	std::vector<std::shared_ptr<IRenderPass>> m_Passes;
};