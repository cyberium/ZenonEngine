#pragma once

#include "EditorInterfaces.h"
#include "SceneNodesSelector.h"

class CSceneEditor
	: public SceneBase
	, public IEditor3DFrame
	, public CSceneNodesSelector
{
public:
	CSceneEditor(IBaseManager& BaseManager);
	virtual ~CSceneEditor();

	void SetEditorUI(IEditorUIFrame* EditorUIFrame);

	// SceneBase
	void Initialize() override;
	void Finalize() override;

	void AddChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode) override;
	void RemoveChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode) override;

	void RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& OwnerNode, const std::shared_ptr<ISceneNode3D>& ChildNode) override;
	void OnMouseClickToWorld(MouseButtonEventArgs::MouseButton& MouseButton, const glm::vec2& MousePosition, const Ray& RayToWorld) override;
	void OnMouseReleaseToWorld(MouseButtonEventArgs::MouseButton& MouseButton, const glm::vec2& MousePosition, const Ray& RayToWorld) override;
	void OnMouseMoveToWorld(MouseButtonEventArgs::MouseButton& MouseButton, const glm::vec2& MousePosition, const Ray& RayToWorld) override;

	void OnPreRender(RenderEventArgs& e) override;

	bool OnWindowKeyPressed(KeyEventArgs& e) override;
	void OnWindowKeyReleased(KeyEventArgs& e) override;

	// IEditor3DFrame
	void LockUpdates() override;
	void UnlockUpdates() override;
	std::shared_ptr<ISceneNode3D> GetRealRootNode3D() const override;
	std::shared_ptr<ISceneNode3D> GetNodeUnderMouse(const glm::ivec2& MousePos) const override;

	// CSceneNodesSelector
	void Selector_OnSelectionChange() override;

protected:
	glm::ivec3 ToBoxCoords(const glm::vec3& Position);
	glm::vec3 FixBoxCoords(const glm::vec3& Position);
	void MoveDraggedNode(const glm::vec3& Position);
	std::shared_ptr<ISceneNode3D> CreateNode(const glm::ivec3& Position, int32 type);

	std::shared_ptr<ISceneNode3D> CreateSelectionBBoxNodeForNode(const std::shared_ptr<ISceneNode3D>& Node);

	void Load3D();
	void LoadUI();

	std::shared_ptr<ISceneNode3D> m_DraggedNode;
	std::shared_ptr<ISceneNode3D> m_RootForSelectedBBoxes;

	bool m_IsSelecting;
	glm::vec2 m_SelectionPrevPos;
	std::shared_ptr<ISceneNodeUI> m_SelectionTexture;

	struct SNode
	{
		int32 X;
		int32 Y;
		int32 Z;
		int32 Type;
		std::shared_ptr<ISceneNode3D> SceneNode;		
	};
	std::vector<SNode> m_Nodes;

	std::shared_ptr<ISceneNode3D> m_Mover;

private:
	IEditorUIFrame* m_EditorUI;
	std::shared_ptr<ISceneNode3D> m_FakeRootNode3D;
};
