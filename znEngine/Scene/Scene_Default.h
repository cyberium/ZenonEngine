#pragma once

#include "Scene/Nodes/UITexture.h"
#include "Passes/Renderer/RendererDeffered.h"

#include "reactphysics3d.h"

class ZN_API CSceneDefault 
	: public SceneBase
{
public:
	CSceneDefault(IBaseManager& BaseManager);
	virtual ~CSceneDefault();

	// IGameState
	void Initialize() override;
	void Finalize() override;

	virtual bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	virtual void OnMouseMoved(const MouseMotionEventArgs & e, const Ray& RayToWorld) override;

	virtual void OnPreRender(RenderEventArgs& e) override;

	// Keyboard events
	virtual bool OnWindowKeyPressed(KeyEventArgs& e) override;
	virtual void OnWindowKeyReleased(KeyEventArgs& e) override;

private:
	void Load3D();
	void GenerateLights(std::shared_ptr<ISceneNode3D> Node, uint32_t numLights);
	void UpdateLights();

private:
	std::shared_ptr<CUITextureNode> m_TextureUI;

private:
	std::shared_ptr<ISceneNode3D> m_RootForBoxes;
	rp3d::DynamicsWorld m_World;
};