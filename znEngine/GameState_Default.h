#pragma once

#include "Passes/CollectLightPass.h"
#include "Passes/SetShaderParameterPass.h"
#include "Passes/ShadowPass.h"
#include "Passes/DefferedRender.h"

#include "SceneFunctional/UI/UITexture.h"

class ZN_API CGameState_World : public CGameState
{
public:
	CGameState_World(IBaseManager * BaseManager, std::shared_ptr<IRenderWindow> RenderWindow, IWindowEvents* WindowEvents);
	virtual ~CGameState_World();

	// IGameState
	bool Init();
	void Destroy();

	virtual void OnRayIntersected(const glm::vec3& Point) override;

	virtual void OnResize(ResizeEventArgs& e) override;

	virtual void OnPreRender(RenderEventArgs& e) override;
	virtual void OnRender(RenderEventArgs& e) override;
	virtual void OnPostRender(RenderEventArgs& e) override;
	virtual void OnRenderUI(RenderEventArgs& e) override;

	// Keyboard events
	virtual void OnKeyPressed(KeyEventArgs& e);
	virtual void OnKeyReleased(KeyEventArgs& e);

private:
	void Load3D();
	void LoadUI();
	void GenerateLights(std::shared_ptr<ISceneNode> Node, uint32_t numLights);
	void CGameState_World::UpdateLights();

private:
	std::shared_ptr<CUITextureNode> m_TextureUI;

private:
	std::shared_ptr<IRenderPass> m_Model_Pass_Opaque;
	std::shared_ptr<IRenderPass> m_Model_Pass_Transperent;

	std::shared_ptr<CCollectLightPass> m_CollectLightPass;

	std::shared_ptr<IStructuredBuffer> m_LightsStructuredBuffer;

	std::shared_ptr<CShadowPass> m_ShadowPass;


	std::shared_ptr<SceneNode3D> m_RootForBoxes;
	std::shared_ptr<CDefferedRender> m_DefferedRenderPass;
};