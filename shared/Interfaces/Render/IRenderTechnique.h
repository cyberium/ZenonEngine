#pragma once

// FORWARD BEGIN
ZN_INTERFACE IRenderPass;
// FORWARD END

ZN_INTERFACE ZN_API IRenderTechnique
{
	virtual ~IRenderTechnique() {}

	virtual uint32 AddPass(std::shared_ptr<IRenderPass> pass) = 0;
	virtual std::shared_ptr<IRenderPass> GetPass(uint32 ID) const = 0;

	virtual void Render(RenderEventArgs& renderEventArgs) = 0;
};