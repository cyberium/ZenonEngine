#pragma once

#include "EditorInterfaces.h"

class CSceneEditor
	: public SceneBase
	, public IEditor3DFrame
{
public:
	CSceneEditor(IBaseManager& BaseManager);
	virtual ~CSceneEditor();

	// IGameState
	void Initialize() override;
	void Finalize() override;

	virtual void OnMouseClickToWorld(MouseButtonEventArgs::MouseButton& MouseButton, const glm::vec2& MousePosition, const Ray& RayToWorld) override;
	virtual void OnMouseMoveToWorld(MouseButtonEventArgs::MouseButton& MouseButton, const glm::vec2& MousePosition, const Ray& RayToWorld) override;

	virtual void OnPreRender(RenderEventArgs& e) override;

	// Keyboard events
	virtual bool OnWindowKeyPressed(KeyEventArgs& e) override;
	virtual void OnWindowKeyReleased(KeyEventArgs& e) override;

protected:

private:
	void Load3D();
	void LoadUI();

private:
	std::shared_ptr<ISceneNode3D> sceneNodeParentt;
};