#pragma once

#include "3D/Passes/DeferredLightingPass.h"

class CGBuffer
{
public:
	CGBuffer(std::shared_ptr<Scene3D> _worldScene);
	virtual ~CGBuffer();

	void Load(uint32 width, uint32 height);
	void Load2(const Viewport& _viewPort);

	std::shared_ptr<IRenderTarget> GetRenderTarget() const;
	std::shared_ptr<DeferredLightingPass> GetPass() const;

private:
	std::shared_ptr<Scene3D>              m_Scene;

	std::shared_ptr<IRenderTarget>        m_RenderTarget;
	std::shared_ptr<DeferredLightingPass> m_Pass;
};