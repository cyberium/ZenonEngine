#pragma once

#include "../Scene/SceneCreateTypelessListPass.h"

class ZN_API CRendererForward
	: public IRenderer
{
public:
	CRendererForward(IBaseManager& BaseManagerl, const std::weak_ptr<IScene>& Scene);
	virtual ~CRendererForward();

	// IRenderer
	uint32 AddPass(std::shared_ptr<IRenderPass> pass) override;
	IRenderPass* GetPass(uint32 ID) const override;
	void Render3D(RenderEventArgs& renderEventArgs) override;
	void RenderUI(RenderEventArgs& renderEventArgs) override;
	void Resize(uint32 NewWidth, uint32 NewHeight) override;

	void Initialize(std::shared_ptr<IRenderTarget> OutputRenderTarget, const Viewport * Viewport);

private:
	std::vector<std::shared_ptr<IRenderPass>> m_Passes;
	std::vector<std::shared_ptr<IRenderPass>> m_UIPasses;

	std::shared_ptr<CSceneCreateTypelessListPass> m_SceneCreateTypelessListPass;
	std::shared_ptr<IMaterialModelPass> m_MaterialModelPass;
	std::shared_ptr<IStructuredBuffer>  m_LightsBuffer;
	size_t							    m_LightsCnt;

private:
	IBaseManager& m_BaseManager;
	IRenderDevice& m_RenderDevice;
	std::weak_ptr<IScene> m_Scene;
};