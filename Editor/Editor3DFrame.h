#pragma once

#include "EditorInterfaces.h"
#include "EditedScene.h"
#include "Editor3DPreviewScene.h"
#include "SceneNodesSelector.h"
#include "Passes/DrawSelectionPass.h"

class CSceneEditor
	: public SceneBase
	, public IEditor3DFrame
	, public CSceneNodesSelector
{
public:
	CSceneEditor(IBaseManager& BaseManager);
	virtual ~CSceneEditor();

	void SetEditorUI(IEditorUIFrame* EditorUIFrame);
	void SetPreviewScene(const std::shared_ptr<CEditor3DPreviewScene>& PreviewScene);

	// SceneBase
	void Initialize() override;
	void Finalize() override;

	void AddChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode) override;
	void RemoveChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode) override;

	void RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& OwnerNode, const std::shared_ptr<ISceneNode3D>& ChildNode) override;
	bool OnMouseClickToWorld(MouseButtonEventArgs::MouseButton& MouseButton, const glm::vec2& MousePosition, const Ray& RayToWorld) override;
	void OnMouseReleaseToWorld(MouseButtonEventArgs::MouseButton& MouseButton, const glm::vec2& MousePosition, const Ray& RayToWorld) override;
	void OnMouseMoveToWorld(MouseButtonEventArgs::MouseButton& MouseButton, const glm::vec2& MousePosition, const Ray& RayToWorld) override;

	void OnPreRender(RenderEventArgs& e) override;

	void OnWindowMouseMoved(MouseMotionEventArgs& e) override;
	bool OnWindowMouseButtonPressed(MouseButtonEventArgs& e) override;
	void OnWindowMouseButtonReleased(MouseButtonEventArgs& e) override;
	bool OnWindowKeyPressed(KeyEventArgs& e) override;
	void OnWindowKeyReleased(KeyEventArgs& e) override;

	// IEditor3DFrame
	IBaseManager& GetBaseManager2() const;
	IRenderDevice& GetRenderDevice2() const;
	void LockUpdates() override;
	void UnlockUpdates() override;
	std::shared_ptr<ISceneNode3D> GetRealRootNode3D() const override;
	std::shared_ptr<ISceneNode3D> GetNodeUnderMouse(const glm::ivec2& MousePos) const override;
	void OnCollectionWidget_ModelSelected(const std::shared_ptr<IModel>& Model) override;
	void DropEvent(const glm::vec2& Position) override;
	void DragEnterEvent(const SDragData& Data) override;
	void DragMoveEvent(const glm::vec2& Position) override;
	void DragLeaveEvent() override;

	// CSceneNodesSelector
	void Selector_OnSelectionChange() override;

protected:
	glm::ivec3 ToBoxCoords(const glm::vec3& Position);
	glm::vec3 FixBoxCoords(const glm::vec3& Position);

	void Load3D();
	void LoadUI();

	void DoMoveNode(const glm::vec2& MousePos);
	void DoDropNodeAndCreateIt();

private:
	bool m_IsDraggingEnabled;
	bool m_IsDraggingPermanentCreation;
	std::shared_ptr<ISceneNode3D> m_DraggedNode;
	std::shared_ptr<ISceneNodeUI> m_DraggerTextUI;

	std::shared_ptr<ISceneNode3D> m_Mover;
	
	bool m_IsSelecting;
	glm::vec2 m_SelectionPrevPos;
	std::shared_ptr<ISceneNodeUI> m_SelectionTexture;
	std::shared_ptr<CDrawSelectionPass> m_DrawSelectionPass;

private:
	IEditorUIFrame* m_EditorUI;
	std::shared_ptr<CEditor3DPreviewScene> m_PreviewScene;
	std::shared_ptr<CEditedScene> m_EditedScene;
};
