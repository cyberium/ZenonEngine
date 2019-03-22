#pragma once

#include "RenderPass.h"

class RenderTechnique : public Object
{
public:
	RenderTechnique();
	virtual ~RenderTechnique();

	virtual unsigned int AddPass(std::shared_ptr<IRenderPass> pass);
	virtual std::shared_ptr<IRenderPass> GetPass(unsigned int ID) const;

	virtual void Render(Render3DEventArgs& renderEventArgs);

	virtual void UpdateViewport(Viewport _viewport);

protected:

private:
	typedef std::vector< std::shared_ptr<IRenderPass> > RenderPassList;
	RenderPassList m_Passes;

};