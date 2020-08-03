#pragma once

#include "EditedScene.h"
#include "Editor3DPreviewScene.h"
#include "Tools/EditorToolsCollection.h"

class CEdtor3DFrame
	: public SceneBase
	, public IEditor3DFrame
	, public IEditor_NodesSelectorEventListener
{
public:
	CEdtor3DFrame(IBaseManager& BaseManager);
	virtual ~CEdtor3DFrame();

	void SetEditorUI(IEditorUIFrame* EditorUIFrame);
	void SetPreviewScene(const std::shared_ptr<CEditor3DPreviewScene>& PreviewScene);

	// SceneBase
	void Initialize() override;
	void Finalize() override;

	void AddChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode) override;
	void RemoveChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode) override;

	void RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& OwnerNode, const std::shared_ptr<ISceneNode3D>& ChildNode) override;

	void OnPreRender(RenderEventArgs& e) override;

	void OnWindowMouseMoved(MouseMotionEventArgs& e) override;
	bool OnWindowMouseButtonPressed(MouseButtonEventArgs& e) override;
	void OnWindowMouseButtonReleased(MouseButtonEventArgs& e) override;
	bool OnWindowKeyPressed(KeyEventArgs& e) override;
	void OnWindowKeyReleased(KeyEventArgs& e) override;

	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs & e, const Ray& RayToWorld) override;

	// IEditorSharedFrame
	IEditor_NodesSelector* GetNodesSelector();

	// IEditor3DFrame
	std::shared_ptr<IScene> GetScene();
	IBaseManager& GetBaseManager2() const;
	IRenderDevice& GetRenderDevice2() const;
	void LockUpdates() override;
	void UnlockUpdates() override;
	void DoEnableTool(ETool Tool);
	std::shared_ptr<IScene> GetEditedScene() const override;
	std::shared_ptr<ISceneNode3D> GetEditedRootNode3D() const override;
	std::shared_ptr<ISceneNode3D> GetNodeUnderMouse(const glm::ivec2& MousePos) const override;
	void OnCollectionWidget_ModelSelected(const std::shared_ptr<IModel>& Model) override;

	void DropEvent(const glm::vec2& Position) override;
	void DragEnterEvent(const SDragData& Data) override;
	void DragMoveEvent(const glm::vec2& Position) override;
	void DragLeaveEvent() override;

	void SetMoverValue(float value) override;

	// IEditor_NodesSelectorEventListener
	void OnSelectNodes() override;

protected: // Game
	void Load3D();

	void SetLights(const std::vector<SLight>& Lights);


private: // Rendering
	std::shared_ptr<IMaterialModelPass> m_MaterialModelPass;
	std::shared_ptr<IStructuredBuffer>  m_LightsBuffer;
	size_t							    m_LightsCnt;

private:
	IEditorUIFrame* m_EditorUI;
	std::shared_ptr<CEditor3DPreviewScene> m_PreviewScene;
	std::shared_ptr<CEditedScene> m_EditedScene;
	CTools m_Tools;
};
