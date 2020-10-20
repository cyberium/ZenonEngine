#pragma once

#include "Scene/Nodes/UITexture.h"
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

private:
	std::shared_ptr<CUITextureNode> m_TextureUI;
	std::shared_ptr<IRenderer> m_ForwardRenderer;
	std::shared_ptr<IRenderer> m_DefferedRenderrer;

private:
	std::shared_ptr<ISceneNode3D> m_RootForBoxes;
	rp3d::DynamicsWorld m_World;
};