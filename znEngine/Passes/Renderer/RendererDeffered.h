#pragma once

#include "../DefferedRender.h"
#include "../DefferedRenderPrepareLights.h"
#include "../DefferedRenderFinal.h"

class ZN_API CRendererDeffered
	: public IRenderer
{
public:
	CRendererDeffered(IBaseManager& BaseManagerl, const std::weak_ptr<IScene>& Scene);
	virtual ~CRendererDeffered();

	// IRenderer
	uint32 AddPass(std::shared_ptr<IRenderPass> pass) override;
	IRenderPass* GetPass(uint32 ID) const override;
	std::shared_ptr<IRenderTarget> GetRenderTarget() const;
	void Render(RenderEventArgs& renderEventArgs) override;
	void Resize(uint32 NewWidth, uint32 NewHeight) override;


	void Initialize(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport);

private:
	std::vector<std::shared_ptr<IRenderPass>> m_Passes;

	std::shared_ptr<IRenderTarget> m_FinalRenderTarget;

	std::shared_ptr<CSceneCreateTypelessListPass> m_SceneCreateTypelessListPass;

	std::shared_ptr<CDefferedRender> m_DefferedRenderPass;
	std::shared_ptr<CDefferedRenderPrepareLights> m_DefferedRenderPrepareLights;
	std::shared_ptr<CDefferedRenderFinal> m_DefferedFinalRenderPass;

private:
	IBaseManager& m_BaseManager;
	IRenderDevice& m_RenderDevice;
	std::weak_ptr<IScene> m_Scene;
};