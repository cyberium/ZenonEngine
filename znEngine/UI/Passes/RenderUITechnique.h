#pragma once

#include "RenderUIPass.h"

// The rendering technique determines the method used to render a scene.
// Typical techniques include Forward, Deferred shading, or ForwardPlus.
// A rendering technique consists of one or more render passes, for example, 
// a pass for rendering shadow maps, a pass for rendering the opaque geometry of
// the scene, a pass for rendering the transparent geometry, and one or more 
// passes for rendering individual post-process effects.
class RenderUITechnique : public Object
{
public:
	RenderUITechnique();
	virtual ~RenderUITechnique();

	virtual void SetPass(std::shared_ptr<IRenderUIPass> pass);

	virtual void RenderUI(RenderUIEventArgs& renderEventArgs);

	virtual void UpdateViewport(Viewport _viewport);

protected:

private:
	std::shared_ptr<IRenderUIPass> m_Pass;
};