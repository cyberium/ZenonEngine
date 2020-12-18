#pragma once

ZN_INTERFACE IEditor;
ZN_INTERFACE IEditorTool;
ZN_INTERFACE IEditorTools;
ZN_INTERFACE IEditorShell;
ZN_INTERFACE IEditorUIFrame;
ZN_INTERFACE IEditor3DFrame;
ZN_INTERFACE IEditor3DPreviewFrame;
ZN_INTERFACE IEditorQtUIFrame;

enum ETool
{
	EToolSelector = 999,
	EToolMover = 10,
	EToolRotator = 11,
	EToolScaler = 12,
	EToolDragger = 14,

	// RTS
	EToolMoverRTS = 20,
	EToolRotatorRTS = 21,
	EToolEditorRTS = 22,
	EToolEditorRTSWaypoints = 23,

	EToolDefault = EToolSelector
};


struct SDragData
{
	CByteBuffer Buffer;
	glm::vec2 ScreenPosition;
	bool IsCtrl;
};


ZN_INTERFACE IEditorTool
{
	virtual ~IEditorTool() {}

	virtual void Enable() = 0;
	virtual void Disable() = 0;
	virtual bool IsEnabled() const = 0;
};

ZN_INTERFACE IEditorTools
{
	virtual ~IEditorTools() {}

	virtual void Initialize() = 0;
	virtual void Enable(ETool Tool) = 0;
	virtual IEditorTool& GetTool(ETool Tool) = 0;
	virtual const IEditorTool& GetTool(ETool Tool) const = 0;
	virtual void DisableAll(ETool ExceptOfTool = EToolDefault) = 0;

	// 3D
	virtual void DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget) = 0;
	virtual bool OnMousePressed(const MouseButtonEventArgs& e, const Ray& RayToWorld) = 0;
	virtual void OnMouseReleased(const MouseButtonEventArgs& e, const Ray& RayToWorld) = 0;
	virtual void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) = 0;
	virtual void OnNodeSelected(const std::shared_ptr<ISceneNode> SelectedNode) = 0;

	// UI
	virtual void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) = 0;
	virtual void DropEvent(const glm::vec2& Position) = 0;
	virtual bool DragEnterEvent(const SDragData& Data) = 0;
	virtual void DragMoveEvent(const glm::vec2& Position) = 0;
	virtual void DragLeaveEvent() = 0;

	template<typename T>
	inline T& GetToolT(ETool Tool)
	{
		return dynamic_cast<T&>(GetTool(Tool));
	}

	template<typename T>
	inline const T& GetToolTConst(ETool Tool) const
	{
		return dynamic_cast<const T&>(GetTool(Tool));
	}
};


typedef std::vector<std::weak_ptr<ISceneNode>> SelectedNodes;

//
// Nodes selector
//
ZN_INTERFACE IEditorToolSelector
{
	virtual ~IEditorToolSelector() {}

	virtual void SelectNode(std::shared_ptr<ISceneNode> Node) = 0;
	virtual void SelectNodes(const std::vector<std::shared_ptr<ISceneNode>>& Nodes) = 0;
	virtual void ClearSelection() = 0;
	virtual void AddNode(std::shared_ptr<ISceneNode> Node) = 0;
	virtual void RemoveNode(std::shared_ptr<ISceneNode> Node) = 0;
	virtual std::shared_ptr<ISceneNode> GetFirstSelectedNode() const = 0;
	virtual bool IsNodeSelected(std::shared_ptr<ISceneNode> Node) const = 0;
	virtual const SelectedNodes& GetSelectedNodes() const = 0;
};
ZN_INTERFACE IEditorToolSelectorEventListener
{
	virtual ~IEditorToolSelectorEventListener() {};

	virtual void OnSelectNode() = 0;
};

enum class EMoverDirection
{
	None = 0,
	X,
	Y,
	Z
};

ZN_INTERFACE IEditorToolMover
{
	virtual ~IEditorToolMover() {}

	virtual glm::vec3 FixBoxCoords(const glm::vec3 & Position) = 0;
	virtual void SetMoverValue(float Value) = 0;
	virtual float GetMoverValue() const = 0;
};

ZN_INTERFACE IEditorToolRotator
{
	virtual ~IEditorToolRotator() {}

	virtual float FixAngle(float Angle) = 0;
	virtual void SetRotatorValue(float Value) = 0;
	virtual float GetRotatorValue() const = 0;
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
	virtual IEditor3DPreviewFrame& Get3DPreviewFrame() const = 0;

	virtual IEditorTools& GetTools() = 0;
	virtual IEditorShell& GetShell() = 0;

	virtual bool IsNodeSelected(std::shared_ptr<ISceneNode> Node) const = 0;
	virtual std::shared_ptr<ISceneNode> GetFirstSelectedNode() const = 0;
	virtual std::vector<std::shared_ptr<ISceneNode>> GetSelectedNodes() const = 0;
};

ZN_INTERFACE IEditorPrivate
{
	virtual ~IEditorPrivate() {}

	virtual void SetUIFrame(IEditorUIFrame* EditorUIFrame) = 0;
	virtual void Set3DFrame(IEditor3DFrame* Editor3DFrame) = 0;
	virtual void Set3DPreviewFrame(IEditor3DPreviewFrame* Editor3DPreviewFrame) = 0;
};

ZN_INTERFACE IEditorShell
{
	virtual ~IEditorShell() {}

	enum EDialogResult
	{
		Yes,
		No,
		Cancel
	};

	virtual EDialogResult ShowYesNoCancelDialog(const char* Text, ...) const = 0;
	virtual void          ShowErrorDialog(const char*, ...) const = 0;
	virtual std::string   ShowLoadFileDialog(std::string DefaultPathAndName) const = 0;
	virtual std::string   ShowSaveFileDialog(std::string DefaultPath) const = 0;
};


ZN_INTERFACE IEditor3DFrame
{
	virtual ~IEditor3DFrame() {}

	virtual IEditor& GetEditor() const = 0;
	virtual bool InitializeEditorFrame() = 0;

	virtual void DoInitializeTools3D() = 0;
	virtual IScene& GetScene() = 0;
	virtual void DoEnableTool(ETool Tool) = 0;

	virtual std::shared_ptr<IScene>     GetEditedScene() const = 0;
	virtual std::shared_ptr<ISceneNode> GetEditedRootNode3D() const = 0;
	virtual std::shared_ptr<ISceneNode> GetEditedNodeUnderMouse(const glm::ivec2& MousePos) const = 0;
};


ZN_INTERFACE IEditor3DPreviewFrame
{
	virtual ~IEditor3DPreviewFrame() {}

	virtual void SetSceneNode(std::shared_ptr<ISceneNode> SceneNode) = 0;
	virtual void SetModel(std::shared_ptr<IModel> Model) = 0;
	virtual void SetTexture(std::shared_ptr<ITexture> Texture) = 0;
	virtual void SetParticleSystem(std::shared_ptr<IParticleSystem> ParticleSystem) = 0;
	virtual void SetMaterial(std::shared_ptr<IMaterial> Material) = 0;
};


ZN_INTERFACE IEditorUIFrame
{
	virtual ~IEditorUIFrame() {}

	virtual IEditor& GetEditor() const = 0;
	virtual bool InitializeEditorFrame() = 0;

	virtual void DoInitializeToolsUI() = 0;
	virtual bool ExtendContextMenu(const std::shared_ptr<ISceneNode>& Node, std::shared_ptr<IPropertiesGroup> PropertiesGroup) = 0;
};
