#pragma once

#include "../Scene/SceneCreateTypelessListPass.h"

#include "PassDeffered_DoRenderScene.h"
#include "PassDeffered_ShadowMaps.h"
#include "PassDeffered_MergeShadowMaps.h"
#include "PassDeffered_RenderUIQuad.h"

class ZN_API CRendererDeffered
	: public RendererBase
{
public:
	CRendererDeffered(IBaseManager& BaseManagerl, IScene& Scene);
	virtual ~CRendererDeffered();

	void Initialize(std::shared_ptr<IRenderTarget> OutputRenderTarget);

private:
	std::shared_ptr<IRenderTarget> CreateHDRRenderTarget(std::shared_ptr<IRenderTarget> OutputRenderTarget);

private:
	std::shared_ptr<IRenderPassCreateTypelessList> m_SceneCreateTypelessListPass;

	std::shared_ptr<CPassDeffered_DoRenderScene> m_RenderScenePass;
	std::shared_ptr<CPassDeffered_ShadowMaps> m_ShadowMapsPass;
	std::shared_ptr<CPassDeffered_MergeShadowMaps> m_MergeShadowMapsPass;
	std::shared_ptr<CPassDeffered_RenderUIQuad> m_Deffered_UIQuadPass;
};