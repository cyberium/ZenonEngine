#pragma once

ZN_INTERFACE IEditorSharedFrame
{
	virtual ~IEditorSharedFrame() {}
};

struct SDragData
{
	std::string Message;
	glm::vec2 Position;
	bool IsCtrl;
};

ZN_INTERFACE IEditor3DFrame
	: public IEditorSharedFrame
{
	virtual ~IEditor3DFrame() {}

	virtual IBaseManager& GetBaseManager2() const = 0;
	virtual IRenderDevice& GetRenderDevice2() const = 0;
	virtual void LockUpdates() = 0;
	virtual void UnlockUpdates() = 0;
	virtual std::shared_ptr<ISceneNode3D> GetRealRootNode3D() const = 0;
	virtual std::shared_ptr<ISceneNode3D> GetNodeUnderMouse(const glm::ivec2& MousePos) const = 0;

	virtual void OnCollectionWidget_ModelSelected(const std::shared_ptr<IModel>& Model) = 0;

	virtual void DropEvent(const glm::vec2& Position) = 0;
	virtual void DragEnterEvent(const SDragData& Data) = 0;
	virtual void DragMoveEvent(const glm::vec2& Position) = 0;
	virtual void DragLeaveEvent() = 0;

	virtual void SetMoverValue(float value) = 0;
};

ZN_INTERFACE IEditorUIFrame
	: public IEditorSharedFrame
{
	virtual ~IEditorUIFrame() {}

	virtual bool ExtendContextMenu(QMenu * Menu, const std::shared_ptr<ISceneNode3D>& Node) = 0;
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
