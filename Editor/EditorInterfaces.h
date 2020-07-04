#pragma once

ZN_INTERFACE IEditorSharedFrame
{
	virtual ~IEditorSharedFrame() {}
};

ZN_INTERFACE IEditor3DFrame
	: public IEditorSharedFrame
{
	virtual ~IEditor3DFrame() {}

	virtual void LockUpdates() = 0;
	virtual void UnlockUpdates() = 0;
	virtual std::shared_ptr<ISceneNode3D> GetRealRootNode3D() const = 0;
	virtual std::shared_ptr<ISceneNode3D> GetNodeUnderMouse(const glm::ivec2& MousePos) const = 0;
};

ZN_INTERFACE IEditorUIFrame
	: public IEditorSharedFrame
{
	virtual ~IEditorUIFrame() {}

	virtual void ExtendContextMenu(QMenu * Menu, const std::shared_ptr<ISceneNode3D>& Node) = 0;
	virtual void OnSceneChanged() = 0;
};
