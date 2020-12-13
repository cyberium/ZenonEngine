#pragma once

#include "EditedScene.h"
#include "Editor3DPreviewScene.h"

class CEditor3DFrame
	: public SceneBase
	, public IEditor3DFrame
	, public IEditorToolSelectorEventListener
	, public ISceneEventsListener // Listener for EditedScene events
{
public:
	CEditor3DFrame(IEditor& Editor, IRenderWindow& RenderWindow);
	virtual ~CEditor3DFrame();

	// SceneBase
	void Initialize() override;
	void Finalize() override;

	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs & e, const Ray& RayToWorld) override;

	// IEditor3DFrame
	IEditor& GetEditor() const override;
	bool InitializeEditorFrame() override;
	void DoInitializeTools3D() override;
	IScene& GetScene() override;
	void DoEnableTool(ETool Tool);
	std::shared_ptr<IScene> GetEditedScene() const override;
	std::shared_ptr<ISceneNode> GetEditedRootNode3D() const override;
	std::shared_ptr<ISceneNode> GetEditedNodeUnderMouse(const glm::ivec2& MousePos) const override;

	// IEditorToolSelectorEventListener
	void OnSelectNode() override;

	// ISceneEventsListener // from edited scene
	void OnSceneNodeAdded(std::shared_ptr<ISceneNode> ParentNode, std::shared_ptr<ISceneNode> ChildNode) override;
	void OnSceneNodeRemoved(std::shared_ptr<ISceneNode> ParentNode, std::shared_ptr<ISceneNode> ChildNode) override;

private:
	IEditor& m_Editor;
	std::shared_ptr<CEditedScene> m_EditedScene;
};
