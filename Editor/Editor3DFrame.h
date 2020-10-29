#pragma once

#include "EditedScene.h"
#include "Editor3DPreviewScene.h"

class CEditor3DFrame
	: public SceneBase
	, public IEditor3DFrame
	, public IEditorToolSelectorEventListener
{
public:
	CEditor3DFrame(IEditor& Editor, IRenderWindow& RenderWindow);
	virtual ~CEditor3DFrame();

	void SetPreviewScene(const std::shared_ptr<CEditor3DPreviewScene>& PreviewScene);

	// SceneBase
	void Initialize() override;
	void Finalize() override;

	void AddChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode) override;
	void RemoveChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode) override;
	void RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode) override;

	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs & e, const Ray& RayToWorld) override;

	// IEditorSharedFrame
	IEditor& GetEditor() const override;
	bool InitializeEditorFrame() override;

	// IEditor3DFrame
	void DoInitializeTools3D() override;
	std::shared_ptr<IScene> GetScene() override;
	void LockUpdates() override;
	void UnlockUpdates() override;
	void DoEnableTool(ETool Tool);
	std::shared_ptr<IScene> GetEditedScene() const override;
	std::shared_ptr<ISceneNode3D> GetEditedRootNode3D() const override;
	std::shared_ptr<ISceneNode3D> GetNodeUnderMouse(const glm::ivec2& MousePos) const override;
	void OnCollectionWidget_ModelSelected(const std::shared_ptr<IModel>& Model) override;

	// IEditorToolSelectorEventListener
	void OnSelectNode() override;

private:
	IEditor& m_Editor;
	std::shared_ptr<CEditor3DPreviewScene> m_PreviewScene;
	std::shared_ptr<CEditedScene> m_EditedScene;
};
