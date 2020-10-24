#pragma once

#include "../PassDeffered_DoRenderScene.h"
#include "../PassDeffered_ProcessLights.h"
#include "../PassDeffered_RenderUIQuad.h"
#include "../Postprocess_HDR.h"

class ZN_API CRendererDeffered
	: public IRenderer
{
public:
	CRendererDeffered(IBaseManager& BaseManagerl, const std::weak_ptr<IScene>& Scene);
	virtual ~CRendererDeffered();

	// IRenderer
	uint32 AddPass(std::shared_ptr<IRenderPass> pass) override;
	IRenderPass* GetPass(uint32 ID) const override;
	void Render3D(RenderEventArgs& renderEventArgs) override;
	void RenderUI(RenderEventArgs& renderEventArgs) override;
	void Resize(uint32 NewWidth, uint32 NewHeight) override;


	void Initialize(std::shared_ptr<IRenderTarget> OutputRenderTarget, const Viewport * Viewport);

private:
	std::shared_ptr<IRenderTarget> CreateHDRRenderTarget(std::shared_ptr<IRenderTarget> OutputRenderTarget, const Viewport * Viewport);

private:
	std::vector<std::shared_ptr<IRenderPass>> m_Passes;
	std::vector<std::shared_ptr<IRenderPass>> m_UIPasses;

	std::shared_ptr<CSceneCreateTypelessListPass> m_SceneCreateTypelessListPass;
	std::shared_ptr<CPassDeffered_DoRenderScene> m_Deffered_ScenePass;
	std::shared_ptr<CPassDeffered_ProcessLights> m_Deffered_Lights;
	std::shared_ptr<CPassDeffered_RenderUIQuad> m_Deffered_UIQuadPass;
	std::shared_ptr<CPassPostprocess_HDR> m_Deffered_HDR;

private:
	IBaseManager& m_BaseManager;
	IRenderDevice& m_RenderDevice;
	std::weak_ptr<IScene> m_Scene;
};