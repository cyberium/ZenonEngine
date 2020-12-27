#pragma once

#include "../Scene/SceneCreateTypelessListPass.h"
#include "../Scene/SceneCreateTypedListsPass.h"

#include "PassForward_DoRenderScene.h"
#include "PassForward_DoRenderSceneInstanced.h"
#include "../RTSGround_Pass.h"
#include "../TerrainPass.h"


class ZN_API CRendererForward
	: public RendererBase
{
public:
	CRendererForward(IBaseManager& BaseManagerl, IScene& Scene);
	virtual ~CRendererForward();

	void Initialize(std::shared_ptr<IRenderTarget> OutputRenderTarget);

private:
	void DoUpdateLights();
	std::shared_ptr<IRenderTarget> CreateHDRRenderTarget(std::shared_ptr<IRenderTarget> OutputRenderTarget);

	std::shared_ptr<IRenderPassCreateTypelessList>        m_SceneCreateTypelessListPass;
	std::shared_ptr<CPassForward_DoRenderScene>           m_MaterialModelPass;
	std::shared_ptr<CTerrainPass>                         m_MaterialTerrainPass;

	std::shared_ptr<IStructuredBuffer>                    m_LightsBuffer;
	size_t                                                m_LightsCnt;
};