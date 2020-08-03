#pragma once

ZN_INTERFACE IEditorTool;
ZN_INTERFACE IEditorTools;
ZN_INTERFACE IEditorUIFrame;
ZN_INTERFACE IEditor3DFrame;
ZN_INTERFACE IEditorQtUIFrame;

enum ETool
{
	EToolSelector,
	EToolMover,
	EToolRotator,
	EToolScaler,
	EToolDragger,
	EToolMoverRTS,
	EToolRotatorRTS,
};

struct SDragData
{
	std::string Message;
	glm::vec2 Position;
	bool IsCtrl;
};


ZN_INTERFACE IEditorTool
{
	virtual ~IEditorTool() {}

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Enable() = 0;
	virtual void Disable() = 0;
	virtual bool IsEnabled() const = 0;
};

ZN_INTERFACE IEditorTools
{
	virtual ~IEditorTools() {}

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Enable(ETool Tool) = 0;
	virtual IEditorTool& GetTool(ETool Tool) = 0;
	virtual void DisableAll() = 0;

	// 3D
	virtual void AddPasses(RenderTechnique& RenderTechnique, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) = 0;
	virtual bool OnMousePressed(const MouseButtonEventArgs& e, const Ray& RayToWorld) = 0;
	virtual void OnMouseReleased(const MouseButtonEventArgs& e, const Ray& RayToWorld) = 0;
	virtual void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) = 0;

	// UI
	virtual void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) = 0;
	virtual void DropEvent(const glm::vec2& Position) = 0;
	virtual void DragEnterEvent(const SDragData& Data) = 0;
	virtual void DragMoveEvent(const glm::vec2& Position) = 0;
	virtual void DragLeaveEvent() = 0;
};


typedef std::vector<std::shared_ptr<ISceneNode3D>> SelectedNodes;

//
// Nodes selector
//
ZN_INTERFACE IEditor_NodesSelector
{
	virtual ~IEditor_NodesSelector() {}

	virtual void SelectNode(std::shared_ptr<ISceneNode3D> Node) = 0;
	virtual void SelectNodes(const SelectedNodes& Nodes) = 0;
	virtual void ClearSelection() = 0;
	virtual void AddNode(std::shared_ptr<ISceneNode3D> Node) = 0;
	virtual void RemoveNode(std::shared_ptr<ISceneNode3D> Node) = 0;
	virtual std::shared_ptr<ISceneNode3D> GetFirstSelectedNode() = 0;
	virtual bool IsNodeSelected(std::shared_ptr<ISceneNode3D> Node) = 0;
	virtual const SelectedNodes& GetSelectedNodes() = 0;
};
ZN_INTERFACE IEditor_NodesSelectorEventListener
{
	virtual ~IEditor_NodesSelectorEventListener() {};

	virtual void OnSelectNode() = 0;
};


//
// Frames intfs
//

ZN_INTERFACE IEditor
{
	virtual ~IEditor() {}

	virtual IBaseManager& GetBaseManager() const = 0;
	virtual IRenderDevice& GetRenderDevice() const = 0;
	virtual IEditorUIFrame& GetUIFrame() const = 0;
	virtual IEditor3DFrame& Get3DFrame() const = 0;
	virtual IEditorTools& GetTools() = 0;

	virtual bool IsNodeSelected(std::shared_ptr<ISceneNode3D> Node) const = 0;
	virtual std::shared_ptr<ISceneNode3D> GetFirstSelectedNode() const = 0;
	virtual const SelectedNodes& GetSelectedNodes() const = 0;
};

ZN_INTERFACE IEditorPrivate
{
	virtual ~IEditorPrivate() {}

	virtual void SetUIFrame(IEditorUIFrame* EditorUIFrame) = 0;
	virtual void Set3DFrame(IEditor3DFrame* Editor3DFrame) = 0;
};

ZN_INTERFACE IEditorSharedFrame
{
	virtual ~IEditorSharedFrame() {}

	virtual IEditor& GetEditor() const = 0;
	virtual bool InitializeEditorFrame() = 0;
};



ZN_INTERFACE IEditor3DFrame
	: public IEditorSharedFrame
{
	virtual ~IEditor3DFrame() {}

	virtual void DoInitializeTools3D() = 0;
	virtual std::shared_ptr<IScene> GetScene() = 0;
	virtual void LockUpdates() = 0;
	virtual void UnlockUpdates() = 0;
	virtual void DoEnableTool(ETool Tool) = 0;

	virtual std::shared_ptr<IScene> GetEditedScene() const = 0;
	virtual std::shared_ptr<ISceneNode3D> GetEditedRootNode3D() const = 0;
	virtual std::shared_ptr<ISceneNode3D> GetNodeUnderMouse(const glm::ivec2& MousePos) const = 0;

	virtual void OnCollectionWidget_ModelSelected(const std::shared_ptr<IModel>& Model) = 0;
};

ZN_INTERFACE IEditorUIFrame
	: public IEditorSharedFrame
{
	virtual ~IEditorUIFrame() {}

	virtual void DoInitializeToolsUI() = 0;
	virtual bool ExtendContextMenu(const std::shared_ptr<ISceneNode3D>& Node, std::string * Title, std::vector<std::shared_ptr<IPropertyAction>> * Actions) = 0;
	virtual void OnSceneChanged() = 0;
};

ZN_INTERFACE IModelCollectionItem
{
	virtual ~IModelCollectionItem() {}

	virtual std::string GetName() const = 0;
	virtual const std::vector<std::shared_ptr<IModelCollectionItem>>& GetChilds() = 0;
	virtual std::shared_ptr<IObject> Object() const = 0;
};

ZN_INTERFACE IModelCollection
{
	virtual ~IModelCollection() {}

	virtual std::shared_ptr<IModelCollectionItem> GetRootItem() const = 0;
};
