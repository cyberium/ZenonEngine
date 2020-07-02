#pragma once

ZN_INTERFACE IEditorSharedFrame
{
	virtual ~IEditorSharedFrame() {}

	virtual void OnSceneNodeSelected(const std::shared_ptr<ISceneNode3D>& SceneNode3D) = 0;
};

ZN_INTERFACE IEditor3DFrame
{
	virtual ~IEditor3DFrame() {}

	virtual void LockUpdates() = 0;
	virtual void UnlockUpdates() = 0;
	virtual std::shared_ptr<ISceneNode3D> GetRealRootNode3D() const = 0;
	virtual std::shared_ptr<ISceneNode3D> GetNodeUnderMouse(const glm::ivec2& MousePos) const = 0;
	virtual void OnSceneNodeSelectedInUIEditor(const std::shared_ptr<ISceneNode3D>& SceneNode3D) = 0;
};

ZN_INTERFACE IEditorUIFrame
{
	virtual ~IEditorUIFrame() {}

	virtual void ExtendContextMenu(QMenu * Menu, const std::shared_ptr<ISceneNode3D>& Node) = 0;
	virtual void OnSceneChanged() = 0;
	virtual void OnSceneNodeSelectedIn3DEditor(const std::shared_ptr<ISceneNode3D>& SceneNode3D) = 0;
};
