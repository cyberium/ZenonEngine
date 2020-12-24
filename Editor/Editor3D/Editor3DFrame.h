#pragma once

#include "EditorInterfaces.h"
#include "EditorQtInterfaces.h"

#include "EditedScene.h"
#include "Editor3DPreviewScene.h"

#include "Dragger.h"

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

protected:
	// Actions
	bool OnContextMenu(const glm::vec2& MousePosition, std::shared_ptr<IPropertiesGroup> Properties);

	// Accept drag
	bool OnDragEnter(const glm::vec2& Position, bool IsCtrl, const CByteBuffer& ByteBuffer);
	bool OnDragMove(const glm::vec2& Position, const CByteBuffer& ByteBuffer);
	bool OnDragDrop(const glm::vec2& Position, const CByteBuffer& ByteBuffer);
	bool OnDragLeave();

private:
	IEditorQtUIFrame& GetEditorQtUIFrame() const;

private:
	IEditor& m_Editor;
	std::shared_ptr<CEditedScene> m_EditedScene;
	CDragger m_Dragger;
};
