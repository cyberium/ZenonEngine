#pragma once

#include "EditorInterfaces.h"

class CSceneEditor
	: public SceneBase
	, public IEditor3DFrame
	, public IEditorSharedFrame
{
public:
	CSceneEditor(IBaseManager& BaseManager);
	virtual ~CSceneEditor();

	void SetEditorUI(IEditorUIFrame* EditorUIFrame);

	// SceneBase
	void Initialize() override;
	void Finalize() override;

	void OnMouseClickToWorld(MouseButtonEventArgs::MouseButton& MouseButton, const glm::vec2& MousePosition, const Ray& RayToWorld) override;
	void OnMouseMoveToWorld(MouseButtonEventArgs::MouseButton& MouseButton, const glm::vec2& MousePosition, const Ray& RayToWorld) override;

	void OnPreRender(RenderEventArgs& e) override;

	bool OnWindowKeyPressed(KeyEventArgs& e) override;
	void OnWindowKeyReleased(KeyEventArgs& e) override;

	// IEditor3DFrame
	std::shared_ptr<ISceneNode3D> GetRealRootNode3D() const override;
	std::shared_ptr<ISceneNode3D> GetNodeUnderMouse(const glm::ivec2& MousePos) const override;
	void OnSceneNodeSelectedInUIEditor(const std::shared_ptr<ISceneNode3D>& SceneNode3D) override;

	// IEditorSharedFrame
	void OnSceneNodeSelected(const std::shared_ptr<ISceneNode3D>& SceneNode3D) override;

protected:
	glm::ivec3 ToBoxCoords(const glm::vec3& Position);
	glm::vec3 FixBoxCoords(const glm::vec3& Position);
	void MoveDraggedNode(const glm::vec3& Position);
	void DoSelectNode(const std::shared_ptr<ISceneNode3D>& Node);
	std::shared_ptr<ISceneNode3D> CreateNode(const glm::ivec3& Position, int32 type);

	void Load3D();
	void LoadUI();

	std::shared_ptr<ISceneNode3D> m_DraggedNode;
	std::shared_ptr<ISceneNode3D> m_SelectedNode;

	struct SNode
	{
		int32 X;
		int32 Y;
		int32 Z;
		int32 Type;
		std::shared_ptr<ISceneNode3D> SceneNode;		
	};
	std::vector<SNode> m_Nodes;

private:
	IEditorUIFrame* m_EditorUI;
};
